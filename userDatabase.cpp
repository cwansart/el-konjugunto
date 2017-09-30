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
#include "userDatabase.h"

// --------------------------------------------------------------------------------------
// Konstruktor
userDatabase::userDatabase()
{
	// Da wir in dieser Version noch keine Funktionalität für andere Sprachen
	// implementiert haben, setzen wir hier einfach mal statisch die Datenbank. ;)
	qDebug() << "Set m_dbName";
	m_dbName = "./db/user";

	// Datenbank laden
	m_link = QSqlDatabase::addDatabase("QSQLITE");
	m_link.setDatabaseName(m_dbName);

	if(m_link.open())
	{
		qDebug() << "Connected!";
	}
	else
	{
		qDebug() << "Connection failed!";
	}
}

// --------------------------------------------------------------------------------------
// bool Query(QString p_query)
// Führt einen Query aus.
bool userDatabase::Query(QString p_query)
{
	m_queryResult.clear();
	
	if(m_link.isOpen())
	{
		m_query = QSqlQuery(p_query);
		return true;
	}
	else
	{
		qDebug() << "No connecion! Please check if the file " + m_dbName + " exists.";
		return false;
	}
}

/*
		QStringList getUserInformation();
		QStringList getUserInformation(QString);
		bool        checkUserData(QString, QString);
*/
// --------------------------------------------------------------------------------------
// QStringList getUserInformation()
// ruft alle Benutzerdaten ab, ohne Passwörter.
QStringList userDatabase::getUserInformation()
{
	if(Query("SELECT id, username FROM users"))
	{
		QStringList currentUser;
		int x;
		while(m_query.next())
		{
			x = 0;
			while(m_query.value(x).toBool())
			{
				currentUser << m_query.value(x).toString();
				x++;
			}
			
			m_queryResult << currentUser;
			currentUser.clear();
		}
		
		if(m_queryResult.size() == 0) return QStringList("NULL");
		return m_queryResult;
	}
	else
	{
		return QStringList("NULL");
	}
}

// --------------------------------------------------------------------------------------
// QStringList getUserInformation(QString p_username)
// ruft alle Benutzerdaten von p_username ab.
QStringList userDatabase::getUserInformation(QString p_username)
{
	if(Query("SELECT * FROM users WHERE username = '" + p_username + "'"))
	{
		int x;
		while(m_query.next())
		{
			x = 0;
			while(m_query.value(x).toBool())
			{
				m_queryResult << m_query.value(x).toString();
				x++;
			}
		}
		
		if(m_queryResult.size() == 0) return QStringList("NULL");
		return m_queryResult;
	}
	else
	{
		qDebug() << "No connection in getUserInformation(QString p_username)";
		return QStringList("NULL");
	}
}

// --------------------------------------------------------------------------------------
// QStringList getUserStatistic(QString p_username)
// Holt alle relevanten Informationen über den Benutzer p_username aus der
// Statistiktabelle und gibt sie als QStringList wieder.
QStringList userDatabase::getUserStatistic(QString p_username)
{
	if(Query("SELECT vocCount, vocRight, vocWrong FROM statistik WHERE username = '" + p_username + "'"))
	{
		int x;
		while(m_query.next())
		{
		//	Muss noch richig gefixed werden
//			while(m_query.value(x).toBool())
			for(x = 0; x != 3; x++)
			{
				m_queryResult << m_query.value(x).toString();
			}
		}
		
		if(m_queryResult.size() == 0)
			return QStringList("NULL");
	}
	else
	{
		qDebug() << "No connection in getUserStatistic(QString)" << __LINE__;
		return QStringList("NULL");
	}
	return m_queryResult;
}


// --------------------------------------------------------------------------------------
// bool checkUserData(QString p_username, QString p_password)
// ruft die Benutzerdaten von p_username ab und vergleicht das Passwort.
bool userDatabase::checkUserData(QString p_username, QString p_password)
{
	QStringList userInformation(getUserInformation(p_username));
	if(userInformation.at(0) == "NULL") return false;
	
	if(p_password == userInformation.at(2)) return true;
	return false;
}

// --------------------------------------------------------------------------------------
// addUser(QString p_username, p_password)
// Fügt einen neuen Benutzer mit dem Namen p_username und dem Password p_password
// hinzu.
// Tritt ein Fehler auf, wird "false" zurückgegeben, andernfalls wird "true" zurückgegeben.
bool userDatabase::addUser(QString p_username, QString p_password)
{
	if(Query("INSERT INTO users(username, password) VALUES('" + p_username + "', '" + p_password + "')"))
	{
		if(m_query.lastError().number() != -1) return false;
		
		if(Query("INSERT INTO statistik(username) VALUES('" + p_username + "')"))
		{
			if(m_query.lastError().number() != -1) return false;
		}
		else
		{
			qDebug() << "No connection in addUser(QString p_username, QString p_password)";
			return false;
		}
	}
	else
	{
		qDebug() << "No connection in addUser(QString p_username, QString p_password)";
		return false;
	}
	
	return true;
}

// --------------------------------------------------------------------------------------
// bool incrVocCount(QString p_username, int p_count = 1)
// Methode zum erhöhen der Vokabeln in der Benutzerstatistik.
// Wird keine Angabe zur Erhöhung getroffen, wird automatisch um 1 erhöht.
bool userDatabase::incrVocCount(QString p_username, int p_count = 1)
{
	if(Query("UPDATE statistik SET vocCount = vocCount + " + QString::number(p_count) + " WHERE username = '" + p_username + "'"))
	{
		if(m_query.lastError().number() != -1) return false;
	}
	else
	{
		qDebug() << "No connection in incrVocCount(QString, int)" << __LINE__;
		return false;
	}
	return true;
}

// --------------------------------------------------------------------------------------
// bool incrRightVocCount(QString p_username, int p_count = 1)
// Erhöht die Anzahl der richtigen Vokabeln in der Benutzerstatistik.
bool userDatabase::incrRightVocCount(QString p_username, int p_count = 1)
{
	if(Query("UPDATE statistik SET vocRight = vocRight + " + QString::number(p_count) + " WHERE username = '" + p_username + "'"))
	{
		if(m_query.lastError().number() != -1) return false;
	}
	else
	{
		qDebug() << "No connection in incrRightVocCount(QString, int)" << __LINE__;
		return false;
	}
	return true;
}

// --------------------------------------------------------------------------------------
// bool incrWrongVocCount(QString p_username, int p_count = 1)
// Erhöht die Anzahl der falschen Vokabeln in der Benutzerstatistik.
bool userDatabase::incrWrongVocCount(QString p_username, int p_count = 1)
{
	if(Query("UPDATE statistik SET vocWrong = vocWrong + " + QString::number(p_count) + " WHERE username = '" + p_username + "'"))
	{
		if(m_query.lastError().number() != -1) return false;
	}
	else
	{
		qDebug() << "No connection in incrWrongVocCount(QString, int)" << __LINE__;
		return false;
	}
	return true;
}


// --------------------------------------------------------------------------------------
// Gibt die letzte SQL Fehlermeldung wieder.
QString userDatabase::getLastSqlError()
{
	return m_query.lastError().text();
}

//----
#if 0
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	userDatabase userDB;

	//Passwort überprüfen.
	qDebug() << userDB.checkUserData("Test", "098f6bcd4621d373cade4e832627b4f6");
	
	//Alle Benutzer und ihre Daten ausgeben
	QStringList allUsers(userDB.getUserInformation());
	for(int x = 0; x < allUsers.size(); x++)
	{
		qDebug() << allUsers.at(x);
	}
	
	//Doppelter User
	userDB.addUser("Test", "test");
	qDebug() << userDB.getLastSqlError();
	
	//Benutzerstatistik ändern
	qDebug() << userDB.incrVocCount("Test");
	qDebug() << userDB.incrVocCount("Test", 4);
	
	qDebug() << userDB.incrRightVocCount("Test");
	qDebug() << userDB.incrRightVocCount("Test", 3);

	qDebug() << userDB.incrWrongVocCount("Test");
	qDebug() << userDB.incrWrongVocCount("Test", 2);

	return app.exec();
}
#endif
