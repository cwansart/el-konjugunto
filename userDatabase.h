/****************************************************************************
**
**  Copyright (C) 2009 Viktor Baum, Timo Kikillus, Alex Meier,
**                     Florian Porrmann, Marcel Pokraka,
**                     Sebastian Steinhorst, Niklas Stumpe,
**                     Christian Wansart, Marcel Wiegand
**
**  This file is part of El Konjungúnto.
**
**  El Konjungúnto is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  El Konjungúnto is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with El Konjungúnto.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QVariant>
// Debug
#include <QDebug>
//Includes für rand()
//#include <cstdlib>
//#include <ctime>

class userDatabase: public QObject
{
	Q_OBJECT
	private:
		QString      m_dbName;
		QString      m_lastError;
		QStringList  m_queryResult;
		QSqlDatabase m_link;
		QSqlQuery    m_query;
		bool         Query(QString);

	public:
		userDatabase();
		QString     getLastSqlError();
		QStringList getUserInformation();
		QStringList getUserInformation(QString);
		QStringList getUserStatistic(QString);
		bool        checkUserData(QString, QString);
		bool        addUser(QString, QString);
		bool        incrVocCount(QString, int);
		bool        incrRightVocCount(QString, int);
		bool        incrWrongVocCount(QString, int);
};

#endif
