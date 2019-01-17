//  FileUtils.h
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//       Sérgio Vieira <sergiosvieira@gmail.com>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifdef WIN32
#define WINDOWS
#elif WIN64
#define WINDOWS
#elif __MINGW64__
#define WINDOWS
#endif

#ifndef __FILEUTILS__
#define __FILEUTILS__

#include <string>
#include <iostream>
#include <sys/stat.h>

#ifdef WINDOWS
//#include <dir.h>
#endif



class FileUtils
{

public:

    static int existsPath(const char* path);
    static int createDirectory(std::string path);

}; //FileUtils

#endif /* __FILEUTILS__ */
