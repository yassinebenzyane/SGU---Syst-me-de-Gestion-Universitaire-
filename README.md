# School Management System

A command-line school management system that allows administrators, teachers, and students to manage student information, teacher information, subjects, and timetables.

## Features

- User authentication with different roles (admin, teacher, student)
- Student management (add, modify, delete, display)
- Teacher management (add, modify, delete, display)
- Subject management (add, modify, delete, display)
- Timetable management (generate, add, modify, delete, display)
- Data persistence using text files

## Project Structure

The project is organized in the following directories:

- `include/`: Header files containing function declarations and structures
- `src/`: Source files containing implementations
- `data/`: Directory for data storage (text files)
- `bin/`: Directory for compiled binary files
- `obj/`: Directory for object files

## How to Compile

Use the provided Makefile to compile the project:

```bash
make
