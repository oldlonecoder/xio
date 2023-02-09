cmake_minimum_required(VERSION 3.13..3.24 FATAL_ERROR)
## Project name
set(FrameWorkName "xio")
## Todo renommer la classe xio! { -> xiobject <- ; instruction ; exe ; }
## Todo Renommer "FrameWorkName" ...ou non...
project(${FrameWorkName} VERSION 0.1.0 LANGUAGES CXX)

include(Master.cmake)
