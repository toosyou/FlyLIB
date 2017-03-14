import numpy as np
import copy
from scipy import ndimage

class NeuronRaw:

    def __init__(self, address_am=None, intensity=None):
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
        elif intensity:
            self.read_from_intensity(intensity)
        return

    # size must be setted beforehand
    def read_from_points(self, points, exaggerate=False):
        # check if size contains any zero
        for i in self.size:
            if i == 0:
                raise ValueError('Neuron must be allocated beforehand.')

        for point in points:
            # round to nestest integer
            coordinate = np.rint(np.array(point) - np.array([self._x_min, self._y_min, self._z_min]) )
            coordinate = coordinate.astype(int)
            # check boundary
            for i, coor in enumerate(coordinate):
                if coor >= self.size[i]:
                    coordinate[i] = self.size[i]-1
                elif coor < 0:
                    coordinate[i] = 0
            if exaggerate:
                for i in range(3):
                    for j in range(3):
                        for k in range(3):
                            new_coor = coordinate + np.array([i-1, j-1, k-1])
                            # check boundary
                            for m, coor in enumerate(new_coor):
                                if coor >= self.size[m]:
                                    new_coor[m] = self.size[m]-1
                                elif coor < 0:
                                    new_coor[m] = 0
                            self.intensity[new_coor[0]][new_coor[1]][new_coor[2]] = 1.0
            else:
                self.intensity[coordinate[0]][coordinate[1]][coordinate[2]] = 1.0

        return

    def clean_intensity(self):
        self.intensity = np.zeros(self.intensity.shape)
        return

    def read_from_intensity(self, intensity, x_min=0, y_min=0, z_min=0):
        self.size = list(intensity.shape)
        self.intensity = copy.deepcopy(intensity)
        self.valid = True
        self._x_min = x_min
        self._x_max = x_min + self.size[0]
        self._y_min = y_min
        self._y_max = y_min + self.size[1]
        self._z_min = z_min
        self._z_max = z_min + self.size[2]
        return

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

    def block(self, start_point, size=(16, 16, 16)):
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

    def point_in_the_center(self, point):
        point_shift = [0, 0, 0]
        point_shift[0] = point[0] - self._x_min
        point_shift[1] = point[1] - self._y_min
        point_shift[2] = point[2] - self._z_min

        for i in range(3):
            if point_shift[i] < self.size[i] * 0.25 or point_shift[i] >= self.size[i] * 0.75:
                return False

        return True

    def points_in_the_center(self, points):
        for point in points:
            if self.point_in_the_center(point):
                return True
        return False

    def normalize(self, rg=[0, 1]):
        rtn = copy.deepcopy(self)
        # find maximum of intensity
        maximum = np.max(rtn.intensity)
        rtn.intensity = rtn.intensity / maximum * (rg[1] - rg[0]) + rg[0]
        return rtn

    def binarize(self, keep_threshold_percentage=0.8):
        maximum = np.amax(self.intensity)
        threshold = maximum * keep_threshold_percentage
        self.intensity = 0.0 + (self.intensity > threshold) * 1
        return None

    def resize(self, size, copy=False, order=0):
        if len(size) != 3:
            raise ValueError('Dimention of size must be 3')

        if copy:
            rtn = copy.deepcopy(self)
        else:
            rtn = self
        magnify_ratio = [0, 0, 0]
        for i in range(3):
            if size[i] == -1:
                magnify_ratio[i] = 1.0
            else:
                magnify_ratio[i] = size[i] / rtn.size[i]

        # zoom x
        rtn._x_min = rtn._x_min * magnify_ratio[0]
        rtn._x_max = rtn._x_max * magnify_ratio[0]
        # zoom y
        rtn._y_min = rtn._y_min * magnify_ratio[1]
        rtn._y_max = rtn._y_max * magnify_ratio[1]
        # zoom z
        rtn._z_min = rtn._z_min * magnify_ratio[2]
        rtn._z_max = rtn._z_max * magnify_ratio[2]

        # zoom intensity with bilinear interpolation
        rtn.intensity = ndimage.zoom(rtn.intensity, magnify_ratio, order=order)

        # new size
        rtn.size = list(rtn.intensity.shape)

        return rtn

    def mirror(self, m=[0, 0, 0]):
        rtn = copy.deepcopy(self)
        for i, yes_mirror_plz in enumerate(m):
            if yes_mirror_plz:
                rtn.intensity = np.flip( rtn.intensity, i )
        return rtn

    def mirror_x(self):
        rtn = copy.deepcopy(self)
        rtn.intensity = np.flip(rtn.intensity, 0);

        return rtn

    def mirror_y(self):
        rtn = copy.deepcopy(self)
        rtn.intensity = np.flip(rtn.intensity, 1);

        return rtn

    def mirror_z(self):
        rtn = copy.deepcopy(self)
        rtn.intensity = np.flip(rtn.intensity, 2);

        return rtn

    def copy(self):
        return copy.deepcopy(self)

    def __getitem__(self, index):
        return self.intensity[index]
