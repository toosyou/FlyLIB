import numpy as np
import copy


class NeuronRaw:

    def __init__(self, address_am=None):
        self.intensity = np.ndarray(shape=(0), dtype=np.float)
        self.valid = False
        self.size = [0, 0, 0]
        self._x_min = 0
        self._x_max = 0
        self._y_min = 0
        self._y_max = 0
        self._z_min = 0
        self._z_max = 0
        if address_am:
            self.valid = self.read_from_am(address_am)

    def read_from_am(self, address_am):
        # open the file
        try:
            in_am = open(address_am, 'r')
        except IOError:
            return False

        line = str()
        times_at_one = 0

        # read information
        while line != False:
            line = in_am.readline()

            # get the size of volume
            if 'define Lattice' in line:
                self.size = list(map(int, line.split()[2:]))
                continue

            # get the boundingbox
            if 'BoundingBox' in line:
                bdbox = line.split()[1:]
                self._x_min = float(bdbox[0])
                self._x_max = float(bdbox[1])
                self._y_min = float(bdbox[2])
                self._y_max = float(bdbox[3])
                self._z_min = float(bdbox[4])
                self._z_max = float(bdbox[5][:-1])
                continue

            # find the start of the data session
            if '@1' in line:
                times_at_one += 1
                if times_at_one == 2:
                    break

        # resize intensity
        self.intensity = np.ndarray(shape=(self.size[0],
                                            self.size[1],
                                            self.size[2]),
                                     dtype=np.float)

        # read intensity
        for z in range(self.size[2]):
            for y in range(self.size[1]):
                for x in range(self.size[0]):
                    self.intensity[x][y][z] = np.float(
                        float(in_am.readline()) / 65535.0)

        in_am.close()
        return True

    def write_am(self, address_am):
        am_out = open(address_am, 'w')

        # output parameters
        am_out.write('# AmiraMesh 3D ASCII 2.0\n\n\n')
        am_out.write('define Lattice %d %d %d\n\n' %
                     (self.size[0], self.size[1], self.size[2]))
        am_out.write('Parameters {\n')
        am_out.write('    Colormap "redblue.icol",\n')
        am_out.write('    Expression "(b)",\n')
        am_out.write('    Content "%dx%dx%d ushort, uniform coordinates",\n' % (
            self.size[0], self.size[1], self.size[2]))
        am_out.write('    SaveInfo "AmiraMesh ZIP",\n')
        am_out.write('    BoundingBox %f %f %f %f %f %f,\n' % (
            self._x_min, self._x_max, self._y_min, self._y_max, self._z_min, self._z_max))
        am_out.write('    CoordType "uniform"\n')
        am_out.write('}\n\n')
        am_out.write('Lattice { ushort Data } @1\n\n')
        am_out.write('# Data section follows\n')
        am_out.write('@1\n')

        # output intensity
        for z in range(self.size[2]):
            for y in range(self.size[1]):
                for x in range(self.size[0]):
                    am_out.write('%d \n' % int(self.intensity[x][y][z]*65535.0))

        am_out.close()

    def block(self, start_point,
                    size=(16, 16, 16)):
        rtn = NeuronRaw()

        # calculate info
        rtn.valid = self.valid
        rtn.size = list(size)
        rtn._x_min = self._x_min + start_point[0]
        rtn._x_max = rtn._x_min + size[0]

        rtn._y_min = self._y_min + start_point[1]
        rtn._y_max = rtn._y_min + size[1]

        rtn._z_min = self._z_min + start_point[2]
        rtn._z_max = rtn._z_min + size[2]

        # copy intensity
        rtn.intensity = np.ndarray(shape=size, dtype=np.float)
        for x in range(start_point[0], start_point[0]+size[0]):
            for y in range(start_point[1], start_point[1]+size[1]):
                for z in range(start_point[2], start_point[2]+size[2]):
                    # coordinate for rtn
                    x_rtn = x - start_point[0]
                    y_rtn = y - start_point[1]
                    z_rtn = z - start_point[2]

                    # zero padding
                    if x < 0 or x >= self.size[0] or\
                        y < 0 or y >= self.size[1] or\
                        z < 0 or z >= self.size[2]:
                        rtn.intensity[x_rtn][y_rtn][z_rtn] = 0.0
                    else:
                        rtn.intensity[x_rtn][y_rtn][z_rtn] = self.intensity[x][y][z]

        return rtn

    def is_empty(self, threshold = 0.000001):

        for x in range(self.size[0]):
            for y in range(self.size[1]):
                for z in range(self.size[2]):
                    if self.intensity[x][y][z] >= threshold:
                        return False
        return True

    def is_in_the_center(self, point):
        point_shift = [0, 0, 0]
        point_shift[0] = point[0] - self._x_min
        point_shift[1] = point[1] - self._y_min
        point_shift[2] = point[2] - self._z_min

        for i in range(3):
            if point_shift[i] < self.size[i] * 0.25 or point_shift[i] >= self.size[i] * 0.75:
                return False

        return True

    def normalize(self):
        rtn = copy.deepcopy(self)
        # find maximum of intensity
        maximum = np.max(rtn.intensity)
        rtn.intensity = rtn.intensity / maximum * 2.0 - 1.0
        return rtn

    def __getitem__(self, index):
        return self.intensity[index]
