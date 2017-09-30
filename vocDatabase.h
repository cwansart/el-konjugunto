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
#ifndef VOCDATABASE_H
#define VOCDATABASE_H

#include <QApplication>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QFile>
// Debug
#include <QDebug>
#include <QSqlError>
//Includes für rand()
#include <cstdlib>
#include <ctime>

class vocDatabase: public QObject
{
	Q_OBJECT

	private:
		QSqlDatabase m_database;
		QString      m_databaseName;
		QStringList  m_queryResult;
		QSqlQuery    m_query;
		bool         m_databaseOpen;
		//bool         m_fetchRowid;
		unsigned int m_queryCount;
		bool         query(QString);
		QStringList  fetchQuery(QString);
		QStringList  fetchQuery();
		int          getFirstQueryAsInt();
		//int          fetchFirstAsInt();
		int          getGroupID(QString);
		
		int          fetchFirstAsInt() { return getFirstQueryAsInt(); }
		bool         query(const char* p) { return query(QString(p)); }
		QStringList  fetchQuery(const char* p) { return fetchQuery(QString(p)); }
		int          getGroupID(const char* p) { return getGroupID(QString(p)); }

	public:
		vocDatabase();
		QStringList getRandomVoc(QString, QString);
		QStringList getAvailableGroups();
		QStringList getGroups();
		QStringList getGroupNames();
		QStringList getAvailableVocs();
		QStringList getAvailableTenses();
		bool        addGroup(QString, QString);
		bool        addVoc(QStringList);
		//void        setFetchRowid(bool);
		bool        removeGroup(QString, bool);
		bool        removeVoc(QString);
		bool        renameGroup(QString, QString);
		QStringList getVoc(QString, QString, QString);
		QStringList getVoc(QString, QString);
		QStringList getTenses(QString);
		bool        updateVoc(QString, QString, QString, char);
		
		QStringList getRandomVoc(const char* p, const char* q) { return getRandomVoc(QString(p), QString(q)); }
		bool        addGroup(const char* p, const char* q) { return addGroup(QString(p), QString(q)); }
		bool        removeGroup(const char* p, bool q) { return removeGroup(QString(p), q); }
		bool        removeVoc(const char* p) { return removeVoc(QString(p)); }
		bool        renameGroup(const char* p, const char* q) { return renameGroup(QString(p), QString(q)); }
		QStringList getVoc(const char* p, const char* q, const char* r) { return getVoc(QString(p), QString(q), QString(r)); }
		QStringList getVoc(const char* p, const char* q) { return getVoc(QString(p), QString(q)); }
		QStringList getTenses(const char* p) { return getTenses(QString(p)); }
		bool updateVoc(const char* p, const char* q, const char* r, char s) { return updateVoc(QString(p), QString(q), QString(r), s); }
		
		/*
		 * Versionsvariable(n)
		 *
		 * Da der ISO-Standard eine direkte Deklaration eines static const char*
		 * Elements verbietet - das wohl damit zu tun hat, dass die Größe des
		 * Elements bekannt sein muss - und der g++ Linker die Deklaration eines
		 * const char[10] Arrays auch nicht zulässt, habe ich die Version in
		 * Gruppen unterteilt.
		 *
		 * Aus der Version 0.1.16 wird
		 *  masterversion  = 0
		 *  subversion     = 1
		 *  releaseversion = 16
		 */
		static const int masterversion  = 0;
		static const int subversion     = 1;
		static const int releaseversion = 12;
};

#endif
