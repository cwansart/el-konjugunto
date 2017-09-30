#ifndef TABLEASCIINAMES_H
#define TABLEASCIINAMES_H

#include <QStringList>

namespace tan
{
	extern QString names[][2];
	extern int tnc1;
	extern int tnc2;

	QString	convert_old_one(QString);
	QStringList convert_new_all(QStringList);
}

#endif
