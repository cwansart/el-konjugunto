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
#include "vocDatabase.h"

bool vocDatabase::updateVoc(QString p_tense, QString p_infinitive, QString p_newVoc, char p_person)
{
	/*
	 * An anderen Stellen hatte ich diese List auch verwendet, jedoch nicht als
	 * QStringList, sondern als Array. Ich bin mir nicht ganz sicher wieso,
	 * aber ich denke es wäre sinnvoll, eine zusätzliche Methode anzulegen, die
	 * Operationen auf diese Liste ermöglichen.
	 */
	const QStringList tenses = QStringList()
		<< "indicativo_presente"
		<< "participio"
		<< "indicativo_imperfecto"
		<< "indicativo_indefinido"
		<< "indicativo_futuro"
		<< "indicativo_condicional"
		<< "subjuntivo_presente"
		<< "subjuntivo_imperfecto"
		<< "imperativo"
		<< "gerundio";
	
	if(!tenses.contains(p_tense))
	{
		qDebug() << "updateVoc: tenses enthaelt keine Zeit namens" << p_tense;
		return false;
	}
		
	if(!query("SELECT ROWID\
	           FROM infinitivo\
	           WHERE verb = '" + p_infinitive + "'"))
	{
		qDebug() << "updateVoc: failed: 1: Vokabel nicht gefunden";
		return false;
	}
	
	int stammid = fetchFirstAsInt();
	
	if(!query("UPDATE " + p_tense + "\
	           SET pers" + QString::number(p_person) + " = '" + p_newVoc + "'\
	           WHERE stamm_id = " + QString::number(stammid)))
	{
		qDebug() << "updateVoc: failed: 2: Konnte nicht updaten";
		return false;
	}
	
	return true;
}

/*
 * Mit getTenses bekommt man alle Zeiten die zu einer bestimmte Vokabelgruppe
 * gehören. Hat man beispielsweise eine Gruppe "Test" in der nur Vokabeln
 * vorhanden sind, die nur in der indicativo_presente Tabelle sind, bekommt
 * man nur indicativo_presente in der QStringList wieder.
 */
QStringList vocDatabase::getTenses(QString p_groupname)
{
	const QString searchTables[] =
	{
		"indicativo_presente",
		"participio",
		"indicativo_imperfecto",
		"indicativo_indefinido",
		"indicativo_futuro",
		"indicativo_condicional",
		"subjuntivo_presente",
		"subjuntivo_imperfecto",
		"imperativo",
		"gerundio"
	};
	
	if(!query("SELECT a.ROWID\
	           FROM infinitivo a\
	           LEFT JOIN gruppen b\
	           ON a.gruppe = b.ROWID\
	           WHERE b.bez = '" + p_groupname + "'"))
	{
		qDebug() << "getTenses: Konnte Vokabeln, die zur Gruppe" << p_groupname;
		qDebug() << "gehoehren, nicht herausfinden.";
		return QStringList();
	}
	
	QStringList vocIDs(fetchQuery());
	QStringList tenses; // Wow bin ich ein Genie! -.- Ein besserer Name für die
	                      // Liste, in der die Zeiten, die zurückgegeben werden,
	                      // ist mir nicht eingefallen!
	
	for(int x = 0; x < vocIDs.count(); x++)
	{
		for(int y = 0; y < (sizeof(searchTables) / sizeof(searchTables[0])); y ++)
		{
			if(!query("SELECT COUNT(*)\
			           FROM " + searchTables[y] + "\
			           WHERE stamm_id = " + vocIDs[x]))
			{
				qDebug() << "getTenses: Fehler beim Zaehlen in der Schleife";
				qDebug() << "x =" << x;
				qDebug() << "y =" << y;
				return QStringList();
			}
			
			if(fetchFirstAsInt() > 0)
			{
				if(!tenses.contains(searchTables[y]))
					tenses.append(searchTables[y]);
			}
		}
	}
	
	return tenses;
}

/*
 * Diese Methode wird gebraucht, zum Editieren der Vokabeln. Es wird keine
 * Zusammensetzung benötigt. Dadurch fällt das perfecto und das
 * pluscuamperfecto weg und stattdessen wird das Participio angezeigt.
 * Dem Benutzer sollte dann eine Information angezeigt werden, das für
 * Perfecto und Pluscuamperfecto nur das Participio geändert werden muss.
 * Man könnte es u.a. im Namen der Auswahl schreiben:
 *  - Presente
 *  - Participio (Perfecto & Pluscuamperfecto)
 *  - Gerundio
 *  - Futuro
 *  ...
 *
 * QString p_groupname
 *
 * (Erläuterung folgt... irgendwan... oô)
 */
QStringList vocDatabase::getVoc(QString p_groupname, QString p_tense)
{
	int groupid = getGroupID(p_groupname);
	
	QString sqlQuery;
	
	if(p_tense == "gerundio")
		sqlQuery = "SELECT b.verb\
		            FROM gerundio a\
		            LEFT JOIN infinitivo b\
		            ON a.stamm_id = b.ROWID\
		            WHERE gruppe = " + QString::number(groupid);
	
	else if(p_tense == "indicativo_perfecto" ||
					p_tense == "indicativo_pluscuamperfecto" ||
					p_tense == "participio")
		sqlQuery = "SELECT b.verb\
		            FROM participio a\
		            LEFT JOIN infinitivo b\
		            ON a.stamm_id = b.ROWID\
		            WHERE gruppe = " + QString::number(groupid);
	
	else
		sqlQuery = "SELECT b.verb\
		            FROM " + p_tense + " a\
		            LEFT JOIN infinitivo b\
		            ON a.stamm_id = b.ROWID\
		            WHERE gruppe = " + QString::number(groupid);
	
	if(!query(sqlQuery))
	{
		qDebug() << "getVoc2: Konnte Query nicht ausfuehren!";
		qDebug() << sqlQuery;
		return QStringList();
	}
	
	return fetchQuery();
}

/*
 * Die Methode getVoc liefert die Formen des übergenden Infinitivs aus der
 * Gruppe p_groupname aus der Zeit p_tense.
 *
 * QString p_infinitive
 *  Das Infinitiv, dessen Formen gesucht werden.
 *
 * QString p_groupname
 *  Name der Vokabelgruppe.
 *
 * QString p_tense
 *  Die Zeitform, aus den die Rückgabe besteht.
 */
QStringList vocDatabase::getVoc(QString p_groupname, QString p_tense, QString p_infinitive)
{
	int groupid = getGroupID(p_groupname);
	int vocid;
	
	if(!query("SELECT ROWID FROM infinitivo WHERE verb = '" + p_infinitive + "'"))
	{
		qDebug() << "getVoc(3): Konnte Infinitiv-ID nicht herausfinden!";
		qDebug() << "p_infinitive =" << p_infinitive;
		return QStringList();
	}
	
	vocid = fetchFirstAsInt();
	if(vocid == 0 || groupid == 0)
		return QStringList();
	
	QString sqlQuery;

	if(p_tense == "imperativo")
		sqlQuery = "SELECT b.pers2, b.pers3, b.pers4, b.pers5, b.pers6\
								FROM imperativo b\
								LEFT JOIN infinitivo a\
								ON b.stamm_id = a.ROWID\
								LEFT JOIN gruppen g\
								ON g.ROWID = a.gruppe\
								AND g.ROWID = " + QString::number(groupid) + "\
								WHERE b.ROWID = " + QString::number(vocid);
	
	else if(p_tense == "gerundio")
		sqlQuery = "SELECT a.verb\n"
               "FROM gerundio a\n"
               "LEFT JOIN infinitivo b\n"
               "ON a.stamm_id = b.ROWID\n"
               "WHERE b.gruppe = " + QString::number(groupid) + "\n"
               "AND a.stamm_id = " + QString::number(vocid);
	
	else if(p_tense == "indicativo_perfecto" ||
					p_tense == "indicativo_pluscuamperfecto" ||
					p_tense == "participio")
		sqlQuery = "SELECT a.verb\n"
		           "FROM participio a\n"
		           "LEFT JOIN infinitivo b\n"
		           "ON a.stamm_id = b.ROWID\n"
		           "WHERE b.gruppe = " + QString::number(groupid) + "\n"
		           "AND b.ROWID = " + QString::number(vocid);
	
	else
		sqlQuery = 	"SELECT a.pers1, a.pers2, a.pers3, a.pers4, a.pers5, a.pers6\
		             FROM " + p_tense + " a\
		             LEFT JOIN infinitivo b\
		             ON a.stamm_id = b.ROWID\
		             WHERE b.gruppe = " + QString::number(groupid) + "\
		             AND a.stamm_id = " + QString::number(vocid);
	
	if(!query(sqlQuery))
	{
		qDebug() << "getVoc(3): Konnte Query nicht durchfuehren!";
		return QStringList();
	}
	
	return fetchQuery();
}

/*
 * getGroupID gibt die ID einer Gruppe wieder, die als Parameter p_groupname
 * übergeben wurde.
 *
 * QString p_groupname
 *  Der Gruppenname, dessen ID herausgefunden werden soll.
 */
int vocDatabase::getGroupID(QString p_groupname)
{
	if(!query("SELECT ROWID FROM gruppen WHERE bez = '" + p_groupname + "'"))
	{
		qDebug() << "getGroupID: Konnte Gruppe" << p_groupname << "nicht finden!";
		return 0;
	}
	
	return fetchFirstAsInt();
}

/*
 * getGroupNames gibt alle Gruppennamen in einer QStringList wieder.
 */
QStringList vocDatabase::getGroupNames()
{
	if(!query("SELECT bez FROM gruppen"))
	{
		qDebug() << "getGroupNames: Konnte Namen nicht selektieren";
		return QStringList();
	}
	
	return fetchQuery();
}

/*
 * Mithife der removeVoc Methode kann man eine bestimmte Vokabel löschen.
 *
 * QString p_voc
 *  Die zu löschende Vokabel in der Infinitivform
 */
bool vocDatabase::removeVoc(QString p_voc)
{
  /*
   * Da der ISO Standard __const__ Elemente in einer Klasse als Membervariable
   * zu deklarieren und ich z.Z. noch keine Lust habe, die Klasse noch einmal
   * umzuschreiben, dass diese Liste wirklich nur noch innerhalb von
   * removeVoc vorhanden zu sein braucht, mache ich es ganz einfach, indem ich
   * sie je nach Gebrauch kopiere... Bäh, wiederliches Verfahren! Q___q
   */
  const QString delTables[] =
	{
		"indicativo_presente",
		"participio",
		"indicativo_imperfecto",
		"indicativo_indefinido",
		"indicativo_futuro",
		"indicativo_condicional",
		"subjuntivo_presente",
		"subjuntivo_imperfecto",
		"imperativo",
		"gerundio"
	};
	
	int delTablesCount = sizeof(delTables) / sizeof(delTables[0]);
	
	if(!query("SELECT ROWID FROM infinitivo WHERE verb = '" + p_voc + "' AND aenderbar = 1"))
	{
		qDebug() << "Abbruch bei SELECT ROWID";
		return false;
	}
	int stammid = fetchFirstAsInt();
	

	for(int x = 0; x < delTablesCount; x++)
	{
		if(!query("DELETE FROM " + delTables[x] + " WHERE stamm_id = " + QString::number(stammid)))
		{
			/*
			 * Hmm... Man muss schauen wie man das macht, da hier mit einem return
			 * false immer abgebrochen würde, sofern nicht überall die Zeiten einer
			 * Vokabel vorhanden wären.
			 */
			//return false;
		}
	}
	
	if(!query("DELETE FROM infinitivo WHERE ROWID = " + QString::number(stammid) + " AND aenderbar = 1"))
	{
		// Gleiches Problem wie oben...
		//return false;
	}
		
	return true;
}

/*
 * Die renameGroup Methode ermöglicht es, einen Gruppennamen zu ändern.
 * Voraussetzung dafür ist aber eine Gruppe, die änderbar ist.
 * 
 * QString p_groupname
 *  Der alte Gruppenname, der geändert werden soll
 *
 * QString p_newGroupname
 *  Der neue Gruppenname
 */
bool vocDatabase::renameGroup(QString p_groupname, QString p_newGroupname)
{
	if(!query("UPDATE gruppen SET bez = '" + p_newGroupname + "' WHERE bez = '" + p_groupname + "' AND aenderbar = 1"))
	{
		qDebug() << "renameGroup: Konnte Gruppennamen nicht aendern.";
		return false;
	}
	
	return true;
}

/*
 * Mit der fetchQuery() Methode lasse ich den letzten mit der Methode query
 * ausgeführten String auswerten und die Ergebnisse in eine QStringList
 * schreiben und zurückgeben.
 */
QStringList vocDatabase::fetchQuery()
{
	if(!m_query.isSelect()) return QStringList();
	
	int colCount = 0;
	while(m_query.next())
	{
		colCount = m_query.record().count();
		for(int x = 0; x < colCount; x++)
		{
			m_queryResult << m_query.value(x).toString();
		}
	}
	
	return m_queryResult;
}

/*
 * removeGroup wird verwendet um eine Vokabelgruppe aus der gruppen-Tabelle
 * zu entfernen. Es steht dabei die Möglichkeit zur Verfügung, die Vokabeln
 * der Gruppe zu löschen oder in die allgemeine Gruppe zu verschieben.
 *
 * QString p_groupname
 *  Der Gruppenname der zu löschenden Vokabelgruppe
 *
 * bool p_delvocs
 *  Falls true übergeben wird, werden die Vokabeln der Gruppe gelöscht.
 *  Bei false werden die Vokabeln in die allgemeine Vokabelgruppe verschoben.
 */
bool vocDatabase::removeGroup(QString p_groupname, bool p_delvocs)
{
	/*
	 * Je nach dem, welcher Wert an p_delvocs übergeben wurde, werden die
	 * Vokabeln der Gruppe gelöscht oder eben in die Gruppe 1, also der 
	 * allgemeinen Gruppe verschoben.
	 *
	 * Als erstes muss einmal die ID der Stammverben herausgefunden werden,
	 * die in der zu löschenden Gruppe sind...
	 */
	if(p_delvocs == true)
	{
		if(!query("SELECT COUNT(*)\
		           FROM gruppen\
		           WHERE bez = '" + p_groupname + "'\
		           AND aenderbar = 1"))
		{
			qDebug() << "removeGroup: Keine Gruppe namens" << p_groupname << "vorhanden!";
			return false;
		}
		
		if(fetchFirstAsInt() == 0)
		{
			qDebug() << "removeGroup: Keine Gruppe namens" << p_groupname << "vorhanden! (2)";
			return false;
		}
		
		/*
		 * Ab hier muss nicht weiter geprüft werden, ob die Gruppe änderbar ist,
		 * da wenn sie es nicht wäre, hätte der vorherige Query 0 ergeben und
		 * false wiedergegeben.
		 */
	
		QString sql("SELECT a.ROWID\
			           FROM infinitivo a\
			           LEFT JOIN gruppen b\
			           ON a.gruppe = b.ROWID\
			           WHERE b.bez = '" + p_groupname + "'\
			           ORDER BY a.ROWID DESC");

		if(!query(sql))
		{
			qDebug() << "Query zur Bestimmung der Vokabeln der zu loeschenden Gruppe";
			qDebug() << "ist fehlgeschlagen! (vocDatabase::removeGroup(QString, bool)\n";
			qDebug() << "Query:\n" << sql;
			return false;
		}
		
		QStringList vocsToDel = fetchQuery();
		int count = vocsToDel.count();
		int stammid;
		
		if(count == 0)
		{
			qDebug() << "vocsToDel.count() == 0";
			//Besser kein return, da es sonst nicht moeglich waere, eine leere
			//Gruppe zu loeschen
			//return false;
		}		
		
		for(int x = 0; x < count; x++)
		{
			qDebug() << "x =" << QString::number(x);
			stammid = vocsToDel.at(x).toInt();
			
			/*
			 * Hier könnte man eventuell die removeVoc Methode anwenden Ist ja, im
			 * Grunde der gleiche Code.
			 */
			 
			const QString delTables[] =
			{
				"indicativo",
				"participio",
				"indicativo_imperfecto",
				"indicativo_indefinido",
				"indicativo_futuro",
				"indicativo_condicional",
				"subjuntivo_presente",
				"subjuntivo_imperfecto",
				"imperativo",
				"gerundio"
			};
	
			int delTablesCount = sizeof(delTables) / sizeof(delTables[0]);
			
			for(int y = 0; y < delTablesCount; y++)
			{
				if(!query("DELETE FROM " + delTables[y] + " WHERE stamm_id = " + QString::number(stammid)))
				{
					//return false;
				}
			}
			
			if(!query("DELETE FROM infinitivo WHERE ROWID = " + QString::number(stammid)))
			{
				return false;
			}
		}
	}
	else
	{
		/*
		 * Anstatt die Vokabeln zu löschen, werden sie hier nur in die 1., also
		 * in die allgemeine Vokabelgruppe verschoben.
		 */
		if(!query("SELECT ROWID FROM gruppen WHERE bez = '" + p_groupname + "'"))
			return false;
		int groupid = fetchFirstAsInt();
		
		if(!query("UPDATE infinitivo SET gruppe = 1 WHERE gruppe = " + groupid))
			return false;
			
		/*
		 * Oh mein Gott war das jetzt simpel. -.-" Und ich wollte das schon so
		 * dämlich in eine Schleife packen wie da oben, beim Löschen der Vokabeln...
		 */
	}
	
	if(!query("DELETE FROM gruppen WHERE bez = '" + p_groupname + "'"))
	{
		qDebug() << "Konnte Gruppe nicht loeschen!";
		return false;
	}
	
	return true;
}

/*
 * Da des öfteren Zahlen aus der Datenbank geholt werden, welche meist nur
 * einer Rückgabe entspricht und man erst die next() Methode ausführen muss,
 * um an den ersten Treffer zu kommen, habe ich diese Methode geschrieben.
 * Sie sorgt außerdem dafür, dass der Rückgabewert auch wirklich eine int ist.
 */
int vocDatabase::getFirstQueryAsInt()
{
	m_query.next();
	return m_query.value(0).toInt();
}

/*
 * Mit der addVoc Methode können neue Vokabeln hinzugefügt werden.
 * Dafür wird die Zeit benötigt, die hinzugefügt werden soll, das Infinitiv,
 * der Gruppenname und die einzelnen Formen des Verbs.
 *
 * Die zu übergebene QStringList hat folgende Struktur:
 *  [0] Zeit
 *  [1] Infinitiv
 *  [2] Gruppe
 *  [3] Pers1
 *  [4] Pers2
 *  [5] Pers3
 *  [6] Pers4
 *  [7] Pers5
 *  [8] Pers6
 *
 * Verbformen für die es nur eine Vokabel gibt (weil sich die eigentliche
 * Verbform aus diesem und einem anderen Verb zusammensetzt):
 *  - indicativo_perfecto
 *  - indicativo_pluscuamperfecto
 *  - gerundio
 *
 * Anzahl der Personen:
 *  - indicativo_presente: 6
 *  - participio: 1
 *  - indicativo_imperfecto: 6
 *  - indicativo_indefinido: 6
 *  - indicativo_futuro: 6
 *  - indicativo_condicional: 6
 *  - subjuntivo_presente: 6
 *  - subjuntivo_imperfecto: 6
 *  - imperativo: 5
 *  - gerundio_auxiliar: 6
 */
bool vocDatabase::addVoc(QStringList p_voc)
{
	int stammid = -1;
	
	/* GruppenID herausfinden und ersetzen */
	if(!query("SELECT ROWID\
	           FROM gruppen\
	           WHERE bez = '" + p_voc.at(2) + "'"))
	{
		qDebug() << "addVoc: Konnte GruppenID nicht herausfinden, bzw. Gruppenname";
		qDebug() << "existiert nicht.";
		return false;
	}
	
	int groupid = fetchFirstAsInt();
	if(groupid == 0)
	{
		qDebug() << "addVoc: groupid ist 0";
		qDebug() << "SELECT ROWID\
	           FROM gruppen\
	           WHERE bez = '" + p_voc.at(2) + "'";
		return false;
	}
	
	/* Einfügen des Infinitivs */
	if(!query("INSERT INTO infinitivo(verb, gruppe)\
	           VALUES('" + p_voc.at(1) + "', " + QString::number(groupid) + ")"))
	{
		/*
		 * Nun ja... Da ein Infinitiv nicht mehrfach vorkommen kann, aber
		 * man wohlmöglich als Benutzer 2 verschiedene Zeiten des gleichen
		 * Infinitivs hinzufügen möchte, wäre es unmöglich hier in der
		 * Methode "false" zurückzugeben. Es sollte ja auch möglich sein,
		 * zu einem Verb, mehrere Zeiten hinzuzufügen.
		 *
		 * Ob hier also noch etwas gemacht werden muss, sollte man später
		 * vielleicht noch entscheiden....
		 * z.B. eine qDebug-Ausgabe.
		 */
		qDebug() << "addVoc: Konnte Ininfitiv nicht einfuegen!";
		qDebug() << "INSERT INTO infinitivo(verb, gruppe)\
	               VALUES('" + p_voc.at(1) + "', " + QString::number(groupid) + ")";
	}
	
	/* Abfrage der dazugehörigen StammID */
	if(!query("SELECT ROWID\
	           FROM infinitivo\
	           WHERE verb = '" + p_voc.at(1) + "'"))
	{
	  /*
	   * Im Grunde ist es nur möglich, hier hinzukommen, wenn die
	   * Datenbank spinnt, das Infinitiv nicht existiert oder die falsche
	   * StammID zugewiesen wurde..
	   */
	  qDebug() << "Query fuer die StammID fehlgeschlagen!";
		return false;
	}
	stammid = getFirstQueryAsInt();
	
	qDebug() << "StammID ist " << QString::number(stammid);
			
	/****************************************************************************/
	/* Einfügen der Vokabeln */
	
	if(p_voc.count() == 9)
	{
		if(p_voc.at(0) == "indicativo_presente" ||
			 p_voc.at(0) == "indicativo_imperfecto" ||
			 p_voc.at(0) == "indicativo_indefinido" ||
			 p_voc.at(0) == "indicativo_futuro" ||
			 p_voc.at(0) == "indicativo_condicional" ||
			 p_voc.at(0) == "subjuntivo_presente" ||
			 p_voc.at(0) == "subjuntivo_imperfecto")
		{
			
			if(!query("INSERT INTO '" + p_voc.at(0) + "'\
			           VALUES(" + QString::number(stammid) + ",\
			                  '" + p_voc.at(3) + "', '" + p_voc.at(4) + "',\
			                  '" + p_voc.at(5) + "', '" + p_voc.at(6) + "',\
			                  '" + p_voc.at(7) + "', '" + p_voc.at(8) + "')"))
			{
				qDebug() << "Query zum Einfuegen fehlgeschlagen 1";
				qDebug() << "INSERT INTO '" + p_voc.at(0) + "'\
			           VALUES(" + QString::number(stammid) + ",\
			                  '" + p_voc.at(3) + "', '" + p_voc.at(4) + "',\
			                  '" + p_voc.at(5) + "', '" + p_voc.at(6) + "',\
			                  '" + p_voc.at(7) + "', '" + p_voc.at(8) + "')";
				return false;
			}
		}
		else
		{
			qDebug() << "Unbekannte Zeit!";
			return false;
		}
	}
	
	else if(p_voc.count() == 8 && p_voc.at(0) == "imperativo")
	{		
		/*
		 * Die zu übergebene QStringList für die Imperativform hat folgende
		 * Struktur, da sie eine Person weniger hat, als die "normalen Formen":
		 *  [0] Zeit
		 *  [1] Infinitiv
		 *  [2] Gruppe
		 *  [3] Pers2
		 *  [4] Pers3
		 *  [5] Pers4
		 *  [6] Pers5
		 *  [7] Pers6
		 */
		if(!query("INSERT INTO imperativo\
		           VALUES(" + QString::number(stammid) + ", '" + p_voc.at(3) + "',\
		                  '" + p_voc.at(4) + "', '" + p_voc.at(5) + "',\
		                  '" + p_voc.at(6) + "', '" + p_voc.at(7) + "')"))
		{
			/* 
			 * Hmmpf... So langsam wird's fad... Aber auf ein Neues:
			 * Hier eventuell eine qDebug-Ausgabe......
			 */
			 qDebug() << "Query zum Einfuegen fehlgeschlagen (imperativo)";
			return false;
		}
	}
	
	else if(p_voc.count() == 4)
	{
		QString tense = p_voc.at(0); // Kompatibilität
		
		if(tense == "indicativo_perfecto" ||
		   tense == "indicativo_pluscuamperfecto")
			tense = "participio";
		
		if(tense == "participio" ||
		   tense == "gerundio")
		{
			/*
			 * Die zu übergebene QStringList für das Participio hat folgende
			 * Struktur:
			 *  [0] Zeit
			 *  [1] Infinitiv
			 *  [2] Gruppe
			 *  [3] Participio / Gerundio
			 */
			if(!query("INSERT INTO " + tense + "\n"
				        "VALUES(" + QString::number(stammid) + ", '" + p_voc.at(3) + "')"))
			{
				/* 
				 * Hmmpf... So langsam wird's fad... Aber auf ein Neues:
				 * Hier eventuell eine qDebug-Ausgabe......
				 */
				qDebug() << "Query zum Einfuegen fehlgeschlagen (participio/gerundio)";
				qDebug() << "INSERT INTO " + tense + "\n"
				        "VALUES(" + QString::number(stammid) + ", '" + p_voc.at(3) + "')";
				return false;
			}
		}
	}
	
	else
	{
		/* Hier vielleicht noch eine qDebug Ausgabe, was passiert ist */
		qDebug() << "Keine passende Zeit angegeben!";
		qDebug() << p_voc.at(0) << "wurde angegeben";
		qDebug() << "p_voc.count() =" << p_voc.count();
		return false;
	}
	
	return true;
}

/*
 * Die getAvailableTenses Methode gibt alle Zeiten in einer QStringList
 * zurück, die vom Programm selbst benutzt werden können.
 * Es wird z.B. kein participio zurückgegeben, da das Participio lediglich
 * zum Zusammensetzen für eine andere Zeit dient.
 */
QStringList vocDatabase::getAvailableTenses()
{
	return fetchQuery("SELECT wert\
	                   FROM einstellungen\
	                   WHERE bez = 'vocTable'");
}

/*
 * addGroup fügt eine neue (Vokabel-)Gruppe hinzu.
 *
 * QString p_bez
 *  Der Name der neuen Gruppe.
 *
 * QString p_beschr
 *  Die Gruppenbeschreibung. Kann auch weggelassen werden.
 */
bool vocDatabase::addGroup(QString p_bez, QString p_beschr = "")
{
	return query("INSERT INTO gruppen(bez, beschr)\
	              VALUES('" + p_bez + "', '" + p_beschr + "')");
}

/*
 * getAvailableVocs gibt in einer QStringList alle Vokabeln zurück, die
 * veränderbar sind. Das dürfte auf alle bis auf haber zutreffen.
 *
 * Man sollte hier eventuell noch ein LIMIT setzen, denn bei mehreren tausend
 * Vokabeln wäre das schon eine ziemlich große QStringList.
 */
QStringList vocDatabase::getAvailableVocs()
{
	return fetchQuery("SELECT id, verb\
	                   FROM infinitivo\
	                   WHERE aenderbar = 'true'");
}

/*
 * getGroups gibt _alle_ Informationen über die Vokabelgruppen zurück.
 * Für die Abfrage der Gruppennamen, besser getGroupNames-Methode verwenden!
 */
QStringList vocDatabase::getGroups()
{
	return fetchQuery("SELECT * FROM gruppen");
}

/*
 * getAvailableGroups ist im Grunde das Gleiche wie getGroups, jedoch werden
 * hier eigens erstellte Gruppen zurückgegeben, also Gruppen, die gelöscht/
 * geändert werden können.
 * Ich bin mir hier aber nicht einmal sicher, ob wir diese Methode je brauchen
 * werden. ^^
 */
QStringList vocDatabase::getAvailableGroups()
{
	return fetchQuery("SELECT bez\
	                   FROM gruppen\
	                   WHERE aenderbar = 'true'");
}

/*
 * fetchQuery liest alle Treffer aus und fügt sie der QStringList m_queryResult
 * hinzu. Zum Schluss wird m_queryResult zurückgegeben.
 *
 * QString p_query
 *  Enthält den auszuführenden Query.
 */
QStringList vocDatabase::fetchQuery(QString p_query)
{
	
	if(!query(p_query)) return QStringList();
	if(!m_query.isSelect()) return QStringList();
	
	int colCount = 0;
	while(m_query.next())
	{
		colCount = m_query.record().count();
		for(int x = 0; x < colCount; x++)
		{
			m_queryResult << m_query.value(x).toString();
		}
	}
	
	return m_queryResult;
}

/*
 * getRandomVoc liefert die Formen und den Infinitiv einer zufällig ausgewählten
 * Vokabel aus der ausgewählten Zeitform.
 *
 * QString p_tense
 *  Die Zeitform, aus der eine zufällige Vokabel zurückgeliefert werden soll.
 */
QStringList vocDatabase::getRandomVoc(QString p_tense, QString p_groupname)
{
	int groupid;
	if(!(groupid = getGroupID(p_groupname)))
	{
		qDebug() << "getRandomVoc: Konnte keine ID zur Gruppe" << p_groupname << "finden!";
		return QStringList();
	}
	
	QString sqlQuery;

	if(p_tense == "indicativo_perfecto" || p_tense == "indicativo_pluscuamperfecto" || p_tense == "participio")
	{
		sqlQuery = "SELECT b.ROWID\n"
			         "FROM participio a\n"
			         "LEFT JOIN infinitivo b\n"
			         "ON a.stamm_id = b.ROWID\n"
			         "WHERE b.gruppe = " + QString::number(groupid);
	}
	else
	{
		sqlQuery = "SELECT b.ROWID\n"
			         "FROM " + p_tense + " a\n"
			         "LEFT JOIN infinitivo b\n"
			         "ON a.stamm_id = b.ROWID\n"
			         "WHERE b.gruppe = " + QString::number(groupid);
	}
	
	if(!query(sqlQuery))
	{
		qDebug() << "getRandomVoc: Konnte " + p_tense + " nicht zaehlen" << "\n";
		qDebug() << "groupid =" << groupid;
		return QStringList();
	}
	
	QStringList vocs;
	vocs = fetchQuery();

	sqlQuery.clear();
	
	if(vocs.count() == 0)
	{
		qDebug() << "getRandomVoc: rowCount = 0";
		return QStringList();
	}
	srand(time(NULL));
	int randVocId = (rand() * m_queryCount) % vocs.count() + 1;
	
	if(p_tense == "imperativo")
		sqlQuery = "SELECT a.verb, b.pers2, b.pers3, b.pers4, b.pers5, b.pers6\n"
		           "FROM imperativo b\n"
		           "LEFT JOIN infinitivo a\n"
		           "ON b.stamm_id = a.ROWID\n"
		           "WHERE b.ROWID = " + vocs.at((randVocId-1));
	
	else if(p_tense == "gerundio")
		sqlQuery = "SELECT a.verb, b.verb, c.pers1, c.pers2, c.pers3, c.pers4, c.pers5, c.pers6\n"
		           "FROM gerundio b\n"
		           "LEFT JOIN infinitivo a\n"
		           "ON b.stamm_id = a.ROWID\n"
		           "LEFT JOIN gerundio_auxiliar c\n"
		           "ON c.ROWID = 1\n"
		           "WHERE b.ROWID = " + vocs.at((randVocId-1));
	
	else if(p_tense == "indicativo_perfecto")
		sqlQuery = "SELECT a.verb, b.verb, c.pers1, c.pers2, c.pers3, c.pers4, c.pers5, c.pers6\n"
		           "FROM participio b\n"
		           "LEFT JOIN infinitivo a\n"
		           "ON b.stamm_id = a.ROWID\n"
		           "LEFT JOIN indicativo_perfecto c\n"
		           "ON c.ROWID = 1\n"
		           "WHERE b.stamm_id = " + vocs.at((randVocId-1));
	
	else if(p_tense == "indicativo_pluscuamperfecto")
		sqlQuery = "SELECT a.verb, b.verb, c.pers1, c.pers2, c.pers3, c.pers4, c.pers5, c.pers6\n"
		           "FROM participio b\n"
		           "LEFT JOIN infinitivo a\n"
		           "ON b.stamm_id = a.ROWID\n"
		           "LEFT JOIN indicativo_pluscuamperfecto c\n"
		           "ON c.ROWID = 1\n"
		           "WHERE b.stamm_id = " + vocs.at((randVocId-1));
	
	else
		sqlQuery = "SELECT b.verb, a.pers1, a.pers2, a.pers3, a.pers4, a.pers5, a.pers6\n"
		           "FROM " + p_tense + " a\n"
		           "LEFT JOIN infinitivo b\n"
		           "ON a.stamm_id = b.ROWID\n"
		           "WHERE b.ROWID = " + vocs.at((randVocId-1));

	if(!query(sqlQuery))
	{
		qDebug() << m_query.lastError().text();
		qDebug() << sqlQuery;
		return QStringList();
	}

	
	if(p_tense == "indicativo_perfecto" ||
		p_tense == "indicativo_pluscuamperfecto" ||
		p_tense == "gerundio")
	{
		while(m_query.next())
		{
			m_queryResult << m_query.value(0).toString();
			for(int x = 2; x < 8; x++)
			{
				m_queryResult << (m_query.value(x).toString() + " "
					+	m_query.value(1).toString());
			}
		}
	}
	else
	{		
		int colCount = 0;
		while(m_query.next())
		{
			colCount = m_query.record().count();
			for(int x = 0; x < colCount; x++)
			{
				m_queryResult << m_query.value(x).toString();
			}
		}
	}
	
	return m_queryResult;
}

/*
 * Die query Methode säubert die Membervariablen, die für einen Query
 * benötigt werden und führt den Query aus. Je nachdem, ob der Query
 * ausgeführt werden konnte, wird false oder true zurückgegeben.
 *
 * QString p_query
 *  Enthält den auszuführenden Query.
 */
bool vocDatabase::query(QString p_query)
{
	if(!m_databaseOpen) return false;
	
	m_queryCount++;
	m_queryResult.clear();
	m_query.clear();
	m_query.prepare(p_query);
	return m_query.exec();
}

/*
 * Der Konstruktor.
 */
vocDatabase::vocDatabase()
{
	m_queryCount = 0;
	
	qDebug("database version %d.%d.%d\n\n", masterversion, subversion, releaseversion);
	
	m_databaseName = "./db/spa";
	if(!QFile::exists(m_databaseName))
	{
		m_databaseOpen = false;
		return;
	}
	
	m_database = QSqlDatabase::addDatabase("QSQLITE");
	m_database.setDatabaseName(m_databaseName);
	if(!m_database.open())
	{
		qDebug() << m_database.lastError().text();
		m_databaseOpen = false;
		return;
	}
	m_databaseOpen = true;
	m_query = QSqlQuery();
}

#if 0
int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	vocDatabase db;
	QStringList tenses;
	
	tenses = db.getRandomVoc("indicativo_pluscuamperfecto", "Allgemein");
	for(int x = 0; x < tenses.count(); x++)
		qDebug() << tenses.at(x);

	return app.exec();
}
#endif
