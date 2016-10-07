import numpy as np


class NeuronRaw:

    def __init__(self, address_am):
        self.read_from_am(address_am)

    def read_from_am(self, address_am):
        # open the file
        self._am_info = list()
        in_am = open(address_am, 'r')
        line = str()
        times_at_one = 0

        # read information
        while line != False:
            line = in_am.readline()
            self._am_info.append(line)

            # get the size of volume
            if 'define Lattice' in line:
                self._size = list(map(int, line.split()[2:]))
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
        self._intensity = np.ndarray(shape=(self._size[0],
                                            self._size[1],
                                            self._size[2]),
                                     dtype=np.uint16)

        # read intensity
        for z in range(self._size[2]):
            for y in range(self._size[1]):
                for x in range(self._size[0]):
                    self._intensity[x][y][z] = np.uint16(
                        int(in_am.readline()))

        in_am.close()

    def write_am(self, address_am):
        am_out = open(address_am, 'w')

        # output parameters
        am_out.write('# AmiraMesh 3D ASCII 2.0\n\n\n')
        am_out.write('define Lattice %d %d %d\n\n' %
                     (self._size[0], self._size[1], self._size[2]))
        am_out.write('Parameters {\n')
        am_out.write('    Colormap "redblue.icol",\n')
        am_out.write('    Expression "(b)",\n')
        am_out.write('    Content "%dx%dx%d ushort, uniform coordinates",\n' % (
            self._size[0], self._size[1], self._size[2]))
        am_out.write('    SaveInfo "AmiraMesh ZIP",\n')
        am_out.write('    BoundingBox %f %f %f %f %f %f,\n' % (
            self._x_min, self._x_max, self._y_min, self._y_max, self._z_min, self._z_max))
        am_out.write('    CoordType "uniform"\n')
        am_out.write('}\n\n')
        am_out.write('Lattice { ushort Data } @1\n\n')
        am_out.write('# Data section follows\n')
        am_out.write('@1\n')

        # output intensity
        for z in range(self._size[2]):
            for y in range(self._size[1]):
                for x in range(self._size[0]):
                    am_out.write('%d \n' % (self._intensity[x][y][z]))

        am_out.close()
