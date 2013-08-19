#include "mel_metrix.h"
#include "mel_wpg_tables.h"
#include "mel_metrix_table.h"

#include <stdio.h>

void mel_metrix_generate(void)
{
	int i;

	for (i = 0; i < MEL_WPG_OpID_TOTALLY; i++)
	{
		int j;
				
		for (j = 0; j < MEL_INSTRUCT_OPERANDS_MAX; j++)
		{
			int cpu = 0;

			WPG_METRIX[i].operands[j].txt_usage = INSTRUCTIONS_METRIX_FXCPU[i].operands[j].txt_usage;
			
			for (cpu = 0; cpu < 2; cpu++)
			{
				WPG_METRIX[i].operands[j].txt_operands[cpu] = INSTRUCTIONS_METRIX_FXCPU[i].operands[j].txt_operands[cpu];
			}
		}
	}

	return;
}

void mel_metrix_printf(void)
{
	int i;

	printf("static WPGMetrixRecord WPG_METRIX[MEL_WPG_OpID_TOTALLY] =\n{\n");

	for (i = 0; i < MEL_WPG_OpID_TOTALLY; i++)
	{
		int j;

		//printf("\t{ //\"%s\" ",INSTRUCTIONS_METRIX_FXCPU[i].name);
		printf("\t{ //\"%s\" ",WPG_OPNAMES[i].name);
		
		if (WPG_METRIX[i].operands[0].txt_usage)
		{
			for (j = 0; j < MEL_INSTRUCT_OPERANDS_MAX; j++)
			{
				int cpu = 0;

				if (WPG_METRIX[i].operands[j].txt_usage)
				{
					printf("\n\n\t\t\t\"%s\",",WPG_METRIX[i].operands[j].txt_usage);
					printf("\t{\t");
					for (cpu = 0; cpu < 2; cpu++)
					{				
						printf("\"%s\"",WPG_METRIX[i].operands[j].txt_operands[cpu]);
						if(cpu!=2-1) printf(",\n\t\t\t\t\t\t");
					}
					printf("}");
				}
				else
				{
					printf("0");
					break;
				}

				if (j != MEL_INSTRUCT_OPERANDS_MAX-1)
				{
					printf(",\t");
				}				
			}
		}
		else
		{
			printf("\n\t\t\t0\t");
		}

		printf("}");

		if (i != MEL_WPG_OpID_TOTALLY-1)
		{
			printf(",");
		}

		printf("\n");
	}

	printf("};\n");

	return;
}

void mel_metrix_stats(void)
{
	int i;
	int n_instractions = MEL_WPG_OpID_TOTALLY;
	int n_done = 0;
	
	for (i = 0; i < MEL_WPG_OpID_TOTALLY; i++)
	{
		//int j;		
		//if (WPG_METRIX[i].operands[0].txt_usage)
		
		if(INSTRUCTIONS_METRIX_FXCPU[i].folder)
		{
			n_done++;
		}
	}

	printf("//Totally: %d instructions; %d done (%.0f %%)\n",
			n_instractions,
			n_done,
			(double)n_done*100/(double)n_instractions
			);

	return;
}

void mel_opnames_printf(void)
{
	int i;

	printf("static WPGOpNameRecord WPG_OPNAMES[MEL_WPG_OpID_TOTALLY] =\n{\n");

	for (i = 0; i < MEL_WPG_OpID_TOTALLY; i++)
	{
		int name_strlen = WPG_OPNAMES[i].namelen;

		printf("\t{\"%s\",",WPG_OPNAMES[i].name);
		{
			int diff = 10 - name_strlen;

			while (diff-- > 0)
			{
				printf(" ");
			}
		}
		printf("%2d}",name_strlen);

		if (i != MEL_WPG_OpID_TOTALLY-1)
		{
			printf(",");
		}

		printf("\n");
	}

	printf("};\n");

	return;
}