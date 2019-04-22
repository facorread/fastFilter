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

/** @file
	* @brief Locale handling utilities. */

#ifndef LOCALE_H
#define LOCALE_H

#include <algorithm>
#include <locale>

namespace sim {
/// Character classification object with << operations to read csv files that use the comma and quotation marks as field delimiters.
/** Usage:
 *     std::locale netLogoLocale(std::locale::classic(), new netLogoCtypeCls); // We recommend only one global object.
 *     std::ifstream inputFile("datafile.csv");
 *     inputFile.imbue(commaLocale);
 */
class netLogoCtypeCls : public std::ctype<char> {
	private:
		/// Character classification table
		mask mTable[table_size];
	public:
		/// Constructor marks the comma as a field delimiter.
		netLogoCtypeCls(size_t refs = 0) : std::ctype<char>(&mTable[0], false, refs) {
				std::copy_n(classic_table(), table_size, mTable);
				/// Mark comma as field separator.
				mTable[static_cast<size_t>(',')] |= space;
				/// Mark double quote mark as a field separator.
				mTable[static_cast<size_t>('"')] |= space;
				/// Unmark space as a field separator.
				mTable[static_cast<size_t>(' ')] &= ~space;
    }
};

} // namespace sim;

#endif // LOCALE_H
