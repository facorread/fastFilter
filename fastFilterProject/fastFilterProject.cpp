/*
	fastFilter: Quick filter for NetLogo BehaviorSpace output
	Copyright (C) 2019 Fabio A. Correa <facorread@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include "locale.h"
#include <sstream>


int main()
{
	std::locale netLogoLocale(std::locale::classic(), new sim::netLogoCtypeCls); // We recommend only one global object.
	const std::filesystem::path inputbase("C:/Users/dirac/NoBackup/2idh/MacPro/temp/5790386/Seans-Mac-Pro.asc.ohio-state.edu/anthropogenic");
	std::filesystem::path ifilename(inputbase);
	ifilename += ".csv";
	std::ifstream ifile(ifilename);
	if(ifile)
	{
		std::filesystem::path ofilename(inputbase);
		ofilename += "-subset.csv";
		std::ofstream ofile(ofilename);
		if(ofile)
		{
			const std::uintmax_t ifilesize(std::filesystem::file_size(ifilename));
			const std::uintmax_t ifilesizeMB(ifilesize / 1000000);
			std::uintmax_t readBytes(0);
			const std::uintmax_t newLineSize(sizeof(ifile.widen('\n')));
			std::string dataString, testString;
			std::istringstream dataStream;
			[[maybe_unused]] std::locale ignoredResult0(dataStream.imbue(netLogoLocale));
			// Copy the file header
			unsigned int lineNumber(1);
			for(; lineNumber < 7; ++lineNumber)
			{
				if(std::getline(ifile, dataString)) {
					// Not checking for ifile.eof() yet; if EOF then this program will exit with error correctly.
					readBytes += dataString.length() + newLineSize;
					if(!(ofile << dataString << std::endl))
					{
						std::cerr << "Error vAFWnphk writing to output file (input line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << "): ";
						perror(nullptr);
						return errno;
					}
				}
				else
				{
					std::cerr << "Error efoI43JE reading from input file (line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << "): ";
					perror(nullptr);
					return errno;
				}
			}
			// Examine the column header
			unsigned int keyColumn(0);
			if (std::getline(ifile, dataString)) {
				// Not checking for ifile.eof(); it should have more than 7 lines.
				readBytes += dataString.length() + newLineSize;
				dataStream.str(dataString);
				bool continueProcessing(true);
				for (; dataStream && !dataStream.eof(); ++keyColumn) {
					if (dataStream >> testString) {
						if (testString == "[step]") {
							continueProcessing = false;
							break;
						}
					}
					else
					{
						std::cerr << "Error a2RBlVjQ reading from input file (line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << ")" << std::endl;
						if (dataStream.eof()) {
							std::cerr << "Is this line the column header? Do you need to change the line number?" << std::endl;
							std::cerr << dataString << std::endl;
						}
						return 1;
					}
				}
				if (continueProcessing) {
					std::cerr << "Error kFclDorS reading from input file (line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << ")" << std::endl;
					if (dataStream.eof()) {
						std::cerr << "Is this the column header? Do you need to change the line number?" << std::endl;
						std::cerr << dataString << std::endl;
					}
					return 1;
				}
				if (!(ofile << dataString << std::endl))
				{
					std::cerr << "Error tU2hlbcY writing to output file (input line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << "): ";
					perror(nullptr);
					return errno;
				}
				++lineNumber;
			}
			else
			{
				std::cerr << "Error Eszqcp3N reading from input file (line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << "): ";
				perror(nullptr);
				return errno;
			}
			// Filter data
			unsigned int writtenLineNumber(lineNumber);
			std::uintmax_t writtenBytes(readBytes);
			std::uintmax_t progressBlocks(0); // This tracker is necessary because progress is not byte-per-byte.
			constexpr std::uintmax_t maxAmenableProgress(std::numeric_limits<std::uintmax_t>::max() / 100);
			for (; ifile && !ifile.eof(); ++lineNumber)
			{
				if (std::getline(ifile, dataString)) {
					const std::uintmax_t processedBytes(dataString.length() + newLineSize);
					readBytes += processedBytes;
					dataStream.str(dataString);
					unsigned int colNumber(0);
					for (; (colNumber < keyColumn) && dataStream && !dataStream.eof(); ++colNumber) {
						dataStream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
					}
					if (colNumber != keyColumn) {
						std::cerr << "Error 32Cj87cm reading from input file (line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << "): colNumber " << colNumber << " keyColumn " << keyColumn << ". Please review the input file." << std::endl;
						return 1;
					}
					unsigned int simStep;
					if (dataStream >> simStep) {
						if ((simStep >= 300) && !(simStep % 10)) {
							writtenBytes += processedBytes;
							++writtenLineNumber;
							if (!(ofile << dataString << std::endl))
							{
								std::cerr << "Error cgxgSKbH writing to output file (input line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << "): ";
								perror(nullptr);
								return errno;
							}
						}
					}
					else
					{
						std::cerr << "Error 4tpFWfaq reading from input file (line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << "): ";
						perror(nullptr);
						return errno;
					}
					const std::uintmax_t newBlocks(readBytes / 50'000'000);
					if (progressBlocks < newBlocks) {
						progressBlocks = newBlocks;
						std::cout << "\33[2K\rInput: " << lineNumber << " lines";
						if (readBytes < maxAmenableProgress) {
							std::cout << " " << (readBytes * 100 / ifilesize) << " %";
						}
						std::cout << " " << (readBytes / 1'000'000) << " MB out of " << ifilesizeMB << " MB -- Output: " << writtenLineNumber << " lines " << (writtenBytes / 1'000'000) << " MB" << std::flush;
					}
				}
				else
				{
					std::cerr << "Error PYKobFTP reading from input file (line " << lineNumber << ") (" << ifile.eof() << ifile.fail() << ifile.bad() << ofile.eof() << ofile.fail() << ofile.bad() << dataStream.eof() << dataStream.fail() << dataStream.bad() << "): ";
					perror(nullptr);
					return errno;
				}
			}
		}
		else
		{
			perror("Error ftwTL0tT opening output file");
			return errno;
		}
	}
	else
	{
		perror("Error 6TTfFxar opening input file");
		return errno;
	}
    return 0;
}
