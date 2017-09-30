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

#ifndef VOKABELADD_H
#define VOKABELADD_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>
#include "vocDatabase.h"
#include "tableAsciiNames.h"

class v_addWindow : public QDialog
{
	Q_OBJECT

	public:
		v_addWindow(QWidget *parent = 0);

	public slots:
		void s_apply();
		void s_tense(QString);

	private:
		QList<QLineEdit*> lle_input;
		QComboBox	*cb_tense;
		QComboBox	*cb_group;
		QLineEdit	*le_name;
		vocDatabase vocDB;
};

#endif
