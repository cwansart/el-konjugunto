/****************************************************************************
**
**	Copyright (C) 2009 Viktor Baum, Timo Kikillus, Alex Meier,
**										 Florian Porrmann, Marcel Pokraka,
**										 Sebastian Steinhorst, Niklas Stumpe,
**										 Christian Wansart, Marcel Wiegand
**
**	This file is part of El Konjungúnto.
**
**	El Konjungúnto is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	El Konjungúnto is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with El Konjungúnto.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "login.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	login window;

	window.setWindowTitle("Konjungunto V 0.1.1");
	window.show();

	app.setApplicationName("Konjungunto");
	app.setApplicationVersion("0.1.1");
	app.setOrganizationName("Bluepanel.de");

	return app.exec();
}
