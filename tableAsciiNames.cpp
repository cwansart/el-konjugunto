#include "tableAsciiNames.h"

namespace tan
{
	QString names[][2] = {
		{ "infinitivo", "Infinitivo" },
		{ "indicativo_presente", "Presente" },
		{ "indicativo_perfecto", "Perfecto" },
		{ "indicativo_pluscuamperfecto", "Pluscuamperfecto" },
		{ "participio", "Perfecto & Pluscuamperfecto" },
		{ "indicativo_imperfecto", "Imperfecto" },
		{ "indicativo_indefinido", "Indefinido" },
		{ "indicativo_futuro", "Futuro" },
		{ "indicativo_condicional", "Condicional" },
		{ "subjuntivo_presente", "Subjuntivo (Presente)" },
		{ "subjuntivo_imperfecto", "Subjuntivo Imperfecto" },
		{ "imperativo", "Imperativo" },
		{ "gerundio", "Gerundio" }
	};

	int tnc1 = sizeof(names) / sizeof(names[0]);
	int tnc2 = sizeof(names[0]) / sizeof(names[0][0]);

	QString convert_old_one(QString tense) 
	{
		for(int i = 0; i < tnc1; i++)
		{
			if(tense == names[i][1])
				tense = names[i][0];
		}

		return tense;
	}

	QStringList convert_new_all(QStringList tenses) 
	{
		for(int i = 0; i < tenses.count(); i++)
		{
			for(int j = 0; j < tnc1; j++)
			{
				if(tenses[i] == names[j][0])
					tenses[i] = names[j][1];
			}
		}

		return tenses;
	}
}
