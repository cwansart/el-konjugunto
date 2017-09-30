/********************************************
*																						*
*																						*
*																						*
********************************************/

#include "abfrage.h"

void abfrage::center(QWidget *widget, int w, int h)
{
	int x, y;
	int screenWidth;
	int screenHeight;

	QDesktopWidget *desktop = QApplication::desktop();

	screenWidth = desktop->width();
	screenHeight = desktop->height();

	x = (screenWidth - w) / 2;
	y = (screenHeight - h) / 2;

	widget->move(x, y);
}

abfrage::abfrage(QStringList vocs, const QString &tense, const QString &name, QWidget *parent)
	:	QWidget(parent)
{
	int width  = 570;
	int height = 500;

	resize(width, height);
	// Sorgt dafür, dass das Fenster nicht verkleinert werden kann
	// (max|min Weite, max|min Höhe)
	setMaximumSize(width, height);
	setMinimumSize(width, height);

	// um den Maximierungs Button abzuschalten
	Qt::WindowFlags flags;
	flags = Qt::Window | Qt::WindowMinimizeButtonHint;
	setWindowFlags(flags);

	QPixmap image(QLatin1String(":/images/el_konjungunto_logo_0.3.png"));

	user = name;
	sl_vocs = vocs;
	counter = 0;
	right = 0;
	falsch  = 0;

	l_vokabel	 = new QLabel("Vokabel:", this);
	//	Label in dem das zu konjugierende Verb steht
	l_verb 		 = new QLabel(this);
	l_tense		 = new QLabel("Zeit:", this);
	l_titel 	 = new QLabel(this);
	l_subtitel = new QLabel("<b>Übungsabteilung</b>", this);
	l_currentTense = new QLabel(("<b>(" + tense + ")</b>"), this);

	pb_input = new QPushButton("Eingabe", this);

	//	pb_nje_k = new QPushButton("\xF1", this);
//	pb_nje_g = new QPushButton("\xD1", this);

//	QGroupBox *gb_nje = new QGroupBox(this);
//	QGridLayout *nje_grid = new QGridLayout(this);

//	nje_grid->addWidget(pb_nje_k, 0, 0, 1, 1);
//	nje_grid->addWidget(pb_nje_g, 0, 1, 1, 1);

//	gb_nje->setLayout(nje_grid);

	QGridLayout	*grid = new QGridLayout(this);

	//	Zentriert den Untertitel
	l_subtitel->setAlignment(Qt::AlignCenter);

	l_titel->setPixmap(image);
	l_titel->setScaledContents(true);

	grid->setSpacing(25);

	//	Fügt die Widgets dem Grid-Layout hinzu
	grid->addWidget(l_titel, 0, 0, 2, 7);
	grid->addWidget(l_subtitel, 2, 0, 1, 5);

	for(int i = 2; i < 8; i++)
	{
		QLabel *l_person;
		if(i <= 4)
			l_person = new QLabel((QString::number(i - 1)+ ".P Sg"), this);
		else
			l_person = new QLabel((QString::number(i - 4)+ ".P Pl"), this);

		l_person->setAlignment(Qt::AlignLeft);
		QLineEdit	*l = new QLineEdit(this);
		lle_input.append(l);
		grid->addWidget(l_person, i + 1, 0, 1, 1);
		grid->addWidget(lle_input[i - 2], i + 1, 1, 1, 3);
	}

//	grid->addWidget(gb_nje, 9, 2, 1, 2);
	grid->addWidget(l_tense, 2, 4, 1, 1);
	grid->addWidget(l_currentTense, 2, 5, 1, 2);
	grid->addWidget(l_vokabel, 3, 4, 1, 1);
	grid->addWidget(l_verb, 3, 5, 1, 2);
	grid->addWidget(pb_input, 9, 5, 1, 2);

	QGroupBox *gb_check = new QGroupBox("Auswertung", this);
	QGridLayout *grid_gb = new QGridLayout(this);

	for(int i = 1; i < 7; i++)
	{
		QLabel *l_person;
		if(i <= 3)
			l_person = new QLabel((QString::number(i)+ ". P SG"), this);
		else
			l_person = new QLabel((QString::number(i - 3)+ ". P  PL"), this);

		l_person->setAlignment(Qt::AlignRight | Qt::AlignBottom);
		QLabel	*l_l = new QLabel(this);
		l_l->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
		QLabel	*l_b = new QLabel(this);
		l_b->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
		ll_result.append(l_l);
		ll_status.append(l_b);
		grid_gb->addWidget(l_person, i - 1, 0, 1, 1);
		grid_gb->addWidget(ll_status[i - 1], i - 1, 1, 1, 1);
		grid_gb->addWidget(ll_result[i - 1], i - 1, 2, 1, 3);
	}

	gb_check->setLayout(grid_gb);
	grid->addWidget(gb_check, 4, 4, 5, 3);

	connect(pb_input, SIGNAL(clicked()), SLOT(s_test()));
//	connect(pb_nje_k, SIGNAL(clicked()), SLOT(s_nje_k()));
//	connect(pb_nje_g, SIGNAL(clicked()), SLOT(s_nje_g()));

	//	Setzt das Layout auf grid

	newvocs();

	setLayout(grid);
	center(this, width, height);
}

void abfrage::newvocs()
{
	if(sl_vocs.empty())
		return;

	clear = false;

	for(int i = 0; i < 6; i++)
		voc_right[i] = false;

	qDebug() << QString::number(sl_vocs.count());
	while(!sl_vocs[0].contains("|=START=|"))
	{
		sl_voc.append(sl_vocs[0]);
		sl_vocs.removeFirst();
	}

	for(int i = 0; i < lle_input.count(); i++)
	{
		lle_input[i]->clear();
		ll_result[i]->clear();
		ll_status[i]->clear();
	}

	sl_vocs.removeFirst();

	if(sl_voc.count() < 7)
	{
		lle_input[0]->setEnabled(false);
		lle_input[0]->setText("Nicht vorhanden");
		ll_result[0]->setText("Nicht vorhanden");
	}

	l_verb->setText("<b>" + sl_voc[0] + "</b>");

	qDebug() << QString::number(sl_vocs.count());
}

/*
void abfrage::s_nje_k()
{
	for(int i = 0; i < 6; i++)
	{
		if(eingaben[i]->isEnabled() == true)
		{
			eingaben[i]->insert("\xF1");
		}
	}

	if(eingaben[1]->isModified() == true)
	{
		eingaben[1]->insert("\xF1");
	}
	if(eingaben[2]->isModified() == true)
	{
		eingaben[2]->insert("\xF1");
	}
}

void abfrage::s_nje_g()
{
	
}
*/

void abfrage::s_test()
{
	if(pb_input->text() == "Nächste Vokabel")
	{
		pb_input->setText("Eingabe");
		newvocs();
		return;
	}

	if(counter > 2 && sl_vocs.empty())
	{
		userDatabase userDB;

		close();
		statistik statistik(QString::number(right + falsch), QString::number(right), QString::number(falsch));
		statistik.setWindowTitle("Statistik");
		statistik.exec();

		userDB.incrVocCount(user, right + falsch);
		userDB.incrRightVocCount(user, right);
		userDB.incrWrongVocCount(user, falsch);

		return;
	}

	/**********************
	*	Datenbank anbindung	*
	**********************/
	QString vocs_db[6];

	int count = sl_voc.count();

	for(int i = 0; i < 6; i++)
	{
		if(count == 6 && i == 5)
			break;

		vocs_db[i] = sl_voc[i + 1];		
	}

	/****************************
	*	Umwandlung der HTML-Tags	*
	****************************/
	for(int i = 0; i < 6; i++)
	{
		if(vocs_db[i].contains("&ntilde;") == true)
			vocs_db[i].replace("&ntilde;", "\xF1");
		if(vocs_db[i].contains("&Ntilde;") == true)
			vocs_db[i].replace("&Ntilde;", "\xD1");
		if(vocs_db[i].contains("&aacute;") == true)
			vocs_db[i].replace("&aacute;", "\xE1");
		if(vocs_db[i].contains("&Aacute;") == true)
			vocs_db[i].replace("&Aacute;", "\xC1");
		if(vocs_db[i].contains("&eacute;") == true)
			vocs_db[i].replace("&eacute;", "\xE9");
		if(vocs_db[i].contains("&Eacute;") == true)
			vocs_db[i].replace("&Eacute;", "\xC9");
		if(vocs_db[i].contains("&iacute;") == true)
			vocs_db[i].replace("&iacute;", "\xED");
		if(vocs_db[i].contains("&Iacute;") == true)
			vocs_db[i].replace("&Iacute;", "\xCD");
		if(vocs_db[i].contains("&oacute;") == true)
			vocs_db[i].replace("&oacute;", "\xF3");
		if(vocs_db[i].contains("&Oacute;") == true)
			vocs_db[i].replace("&Oacute;", "\xD3");
		if(vocs_db[i].contains("&uacute;") == true)
			vocs_db[i].replace("&uacute;", "\xFA");
		if(vocs_db[i].contains("&Uacute;") == true)
			vocs_db[i].replace("&Uacute;", "\xDA");
		if(vocs_db[i].contains("&uuml;") == true)
			vocs_db[i].replace("&uuml;", "\xFC");
		if(vocs_db[i].contains("&Uuml;") == true)
			vocs_db[i].replace("&Uuml;", "\xDC");
	}

	QPixmap kreuz(QLatin1String(":/images/kreuz.png"));
	QPixmap haken(QLatin1String(":/images/haken.png"));


	int j = 0;

	for(int i = 0; i < 6; i++)
	{
		if(count == 6)
		{
			if(i == 5)
				break;

			if(j == 0)
				j++;
		}

		if(lle_input[j]->text() != vocs_db[i])
		{
			ll_status[j]->setPixmap(kreuz);
			if(counter < 2)
				ll_result[j]->setText("<font color=\"red\"><b>Falsch</b><\font>");
			else
			{
				ll_result[j]->setText("<font color=\"red\"><b>" + sl_voc[i + 1] + "</b><\font>");
				falsch++;
			}
		}
		else
		{
			if(voc_right[i] == false)
			{
				ll_status[j]->setPixmap(haken);
				ll_result[j]->setText("<font color=\"green\"><b>- Richtig<\b><\font>");
				voc_right[i] = true;
				right++;

				if((right % 6 == 0 || (count == 6 && right % 5 == 0)) && right > 0)
					clear = true;
			}
		}
		j++;
	}

	counter++;
	if(counter > 2 ||  clear == true)
	{
		sl_voc.clear();
		counter = 3;
		if(!sl_vocs.empty())
		{
			counter = 0;
			pb_input->setText("Nächste Vokabel");
		}
		else
			pb_input->setText("Schließen");
		
	}
}
