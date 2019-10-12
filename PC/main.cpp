/*
 * Copyright (C) 2017  Silvano Seva silseva@fastwebnet.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <regex>
#include <string>
#include "serialport.h"

#define REGX_TAG "counter_val"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout << "Too few arguments provided, quitting" << endl;
        cout << "Usage: contapersone serialPort templateFileName outputFileName"
             << endl;
        return 1;
    }

    ifstream templateFile;
    ofstream outputFile;
    SerialPort *seriale;

    try
    {
        seriale = new SerialPort(argv[1]);
    }
    catch (exception& e)
    {
        e.what();
        getchar();
        return -1;
    }

    templateFile.open(argv[2]);
    outputFile.open(argv[3]);

    if (!templateFile.is_open())
    {
        cout << "this is bad, template file is missing" << endl;
        templateFile.close();
        outputFile.close();
        delete seriale;
        return -1;
    }

    regex reg(REGX_TAG);

    while (1)
    {

        uint32_t value = seriale->requestValue();
        string line;
        while (getline(templateFile, line))
        {
            outputFile << regex_replace(line, reg, to_string(value)) << endl;
        }

        // rewind files for the next run
        templateFile.clear();
        templateFile.seekg(0, ios::beg);

        outputFile.seekp(0, ios::beg);

        // sleep for 500ms aka 500.000us
        usleep(500 * 1000);
    }

    outputFile.close();
    templateFile.close();
    delete seriale;

    return 0;
}
