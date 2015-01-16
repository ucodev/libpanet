/**
 * @file config.h
 * @brief Portable Abstracted Network Library (libpanet)
 *        Configuration header
 *
 * Date: 16-01-2015
 * 
 * Copyright 2012-2015 Pedro A. Hortas (pah@ucodev.org)
 *
 * This file is part of libpanet.
 *
 * libpanet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libpanet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpanet.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef LIBPANET_CONFIG_H
#define LIBPANET_CONFIG_H

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
 #ifndef COMPILE_WIN32
  #define COMPILE_WIN32
 #endif
 #if BUILDING_DLL
  #define DLLIMPORT __declspec(dllexport)
 #else
  #define DLLIMPORT __declspec(dllimport)
 #endif
#endif

#endif

