/* $ DVCS ID: $jer|,523/lhos,KYTP!41023161\b"?" <<= NÃO DELETE! */

/*  ddate.c .. converte o calendário chato e padrão pelo maravilhoso 
    calendário Discordiano ><
    Escrito 65º dia de Consequências por Druel, o Cáotico, 
    vulgo Jeremy Johson mpython@gnu.ai.mit.edu
        28 Sever St Apt #3
        Worcester MA 01609
    
    Eu não sou responsável por esse programa zoar nada (exceto sua mente
    Eu sou responsável por isso)

    (k) YOLD 3161 e todo tempo antes e depois.
    Reutilize, recicle como quiser.
    Esse programa está em dominio público. Distribua gratuitamente.
    Ou não.

    Majorietariamente hackeado, extendido e bugotificado/desbugado em
    Docemanhã, 42 de Burocracia, 3161 YOLD, por LeeH:. O:. Smith, KYTP,
    vulgo Andrew Bulhakm, acb@dev.null.org
    
    Um pouco hackeado e craqueado por um amável fogazinho a lenha em
    HoradoBum, 53º dia da Burocracia em 3179 YOLD, por Chaplain Nyan,
    O Mais Sábio, vulgo Dan Dart, ntw@dandanrt.com.uk

    Histórico:
    Bureflux 3161:  Primeiro lançamento da melhora do ddate com o formato de strings
    59 Brc, 3161:   Divisão das opções PRAISE_BOB e KILL_BOB, outras mudanças.
    53 Brc, 3179:   Consertado a conversão das datas gregorianas antes de 1167 YOLD.

    22-02-1999 Arkadiusz Miskiewicz <misiek@pld.ORG.PL>
    - Adicionou Suporte de linguagem nativo

    17-03-2000 Burt Golzman <misiek@pld.ORG.PL>
    - Adicionou verificação de intervalo por datas.

    07-06-2014 William Woodruff <william@tuffbizz.com>
    - Removeu a dependencia local do gettext

    15º de Confusão, 3180:
    - Chamou os adoradores da Fruta errada.

    57º de Confusão, 3187.
    - Tradução Português BR
    

    Traduzi pois não achei nenhum conversor de datas para shell
    em PT-BR e como esse funciona muito bem resolvi traduzi-lo.
    Peço desculpas (ou não) caso tenha algum erro na tradução e
    cabe a você decidir se ela está certa ou não.
    Pois como se Diz no PENTARROTO, você está proibido de acreditar nisso que está lendo.
    Não traduzi o cód pois é de conveção da raça dos programadores 
    que o programa deve ser em inglês.
    


    CINCO TONELADAS DE LINHO
*/

/* Opções de configuração  VVVVV   LEIA ISSO!!! */

/*Se você quer que o ddate(1) mostre a data no mesmo formato que o ddate original
 * do Druel no modo imediato, defina OLD_IMMEIATE_FMT
 */

#define OLD_IMMEDIATE_FMT

/* Se você quer usar o formato aneristico de datas dos USA (m-d-a) 
 * defina US_FORMAT
 */

/*#define US_FORMAT*/

/* Se você é ideologicamente, teologicamente ou contrário de qualquer modo
 * a Igreja do SubGênio e não quer que a sua cópia do ddate(1) tenha o
 * código de contagem para o Dia-X, comente o KILL_BOB 
 */

#define KILL_BOB 13013

/* Se você quer que o ddate contenha os slogans do SubGênio, defina PRAISE_BOB */

/*#define PRAISE_BOB 13013*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

// Um jeitinho de incluir e definir o c.h como antigamente
#ifndef ARRAY_SIZE
# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
#endif

/* &a[0] degrades to a pointer: um tipo diferente de array para array */
# define __must_be_array(a) \
	BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(__typeof__(a), __typeof__(&a[0])))

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

/* work around hacks for standalone package */
#define PACKAGE "ddate"
#define PACKAGE_STRING "Stand Alone"

#ifndef __GNUC__
#define inline /* foo */
#endif

#ifdef KILL_BOB
int xday_countdown(int yday, int year);
#endif


/* Strings constantes */

char *day_long[5] = { 
    "Docemanhã", "HoradoBum", "DiaPicante", "Espinho-Espinho", "Laranja Poente"
};

char *day_short[5] = {"DM","HdB","DP","EE","LP"};

char *season_long[5] = { 
    "Caos", "Discórdia", "Confusão", "Burocracia", "Consequências"
};

char *season_short[5] = {"Caos", "Dsc", "Cfs", "Brc", "Csq"};

char *holyday[5][2] = { 
    { "Mungday", "Caosflux" },
    { "Mojoday", "Discflux" },
    { "Syaday",  "Conflux" },
    { "Zaraday", "Bureflux" },
    { "Maladay", "Conslux" }
};

struct disc_time {
    int season; /* 0-4 */
    int day; /* 0-72 */
    int yday; /* 0-365 */
    int year; /* 3066- */
};

char *excl[] = {
    "Hail Eris!", "Todos Saudem a Discórdia!", "Kallisti!", "Fnord.", "Ou não.",
    "Wibble.", "Pzat!", "P'tang!", "Frink!",
#ifdef PRAISE_BOB
    "Slack!", "Saudem \"Bob\"!", "Ou... me mate.",
#endif /* PRAISE_BOB */
    /* Frases aleatórias da internet e outros lugares. Sinta-se a vontade para
       adicionar novas, excluir as existentes, inventar (depois de consultar
       as autoridades relevantes e/ou seu Papa pessoal). */
    "Grudnuk precisa de sustento!", "Mantenha a lasanha voando!!",
    "Você é o que você vê..","Você está proibido de acreditar no que lê.",
    "É ou não é?", "Está afirmação é falsa.",
    "Mentiras e calúnias, meu bom senhor!", "Hee hee hee!",
#if defined(linux) || defined (__linux__) || defined (__linux)
    "Hail Eris, Hackeie o Linux!",
#elif defined(__APPLE__)
    "Essa não é a verdadeira fruta da Discórdia.",
#endif
    ""
};

char default_fmt[] = "%{%A, %d de %B, %Y YOLD. %NCelebramos %H%}!!";
char *default_immediate_fmt=
#ifdef OLD_IMMEDIATE_FMT
"Hoje é %{%A, %e de %B%} do ano %Y YOLD. %NCelebramos %H!"
#else
default_fmt
#endif
;

#define DY(y) (y+1166)

static inline char *ending(int i) {
	return "º";
    /*return *i/10==1?"th":(i%10==1?"st":(i%10==2?"nd":(i%10==3?"rd":"th")));*/
    /* Para os números cardinais americanos descomente essa a linha acima e comente a return "º"*/
}

static inline int leapp(int i) {
	return (!(DY(i)%4))&&((DY(i)%100)||(!(DY(i)%400)));
}

/* Seleciona uma String random */
static inline char *sel(char **strings, int num) {
	return(strings[random()%num]);
}

void print(struct disc_time,char **); /* old */
void format(char *buf, const char* fmt, struct disc_time dt);
/* lê o arquivo da fortuna */
int load_fortunes(char *fn, char *delim, char** result);

struct disc_time convert(int,int);
struct disc_time makeday(int,int,int);

int
main (int argc, char *argv[]) {
    time_t t;
    struct tm *eris;
    int bob,raw;
    struct disc_time hastur;
    char schwa[23*17], *fnord=0;
    int pi;
    char *progname, *p;

    progname = argv[0];
    if ((p = strrchr(progname, '/')) != NULL)
	progname = p+1;

    srandom(time(NULL));
    /* faça os "args" aqui */
    for(pi=1; pi<argc; pi++) {
	switch(argv[pi][0]) {
	case '+': fnord=argv[pi]+1; break;
	case '-': 
	    switch(argv[pi][1]) {
	    case 'V':
		printf(("%s (%s)\n"), progname, PACKAGE_STRING);
	    default: goto usage;
	       }
	default: goto thud;
	   }
    }

  thud:
    if (argc-pi==3){ 
	int moe=atoi(argv[pi]), larry=atoi(argv[pi+1]), curly=atoi(argv[pi+2]);
	hastur=makeday(
#ifdef US_FORMAT
	    larry,moe,
#else
	    larry,moe,
#endif
	    curly);
	if (hastur.season == -1) {
		printf("Data Inválida -- ou fora de alcance\n");
		return -1;
	}
	fnord=fnord?fnord:default_fmt;
    } else if (argc!=pi) { 
      usage:
	fprintf(stderr,("Use: %s [+formato] [dia mês ano]\n"), argv[0]);
	exit(1);
    } else {
	t= time(NULL);
	eris=localtime(&t);
	bob=eris->tm_yday; /* Dias desde Jan 1. */
	raw=eris->tm_year; /* Anos desde 1980 */
	hastur=convert(bob,raw);
	fnord=fnord?fnord:default_immediate_fmt;
    }
    format(schwa, fnord, hastur);
    printf("%s\n", schwa);
   
    return 0;
}

void format(char *buf, const char* fmt, struct disc_time dt)
{
    int tib_start=-1, tib_end=0;
    int i, fmtlen=strlen(fmt);
    char *bufptr=buf;

/*    fprintf(stderr, "format(%p, \"%s\", dt)\n", buf, fmt);*/

    /* primeiro, encontra a extensão da área do 
        dia do Sto. Tiby, se definida*/
    for(i=0; i<fmtlen; i++) {
	if(fmt[i]=='%') {
	    switch(fmt[i+1]) {
	    case 'A':
	    case 'a':
	    case 'd':
	    case 'e':
		if(tib_start>0)	    tib_end=i+1;
		else		    tib_start=i;
		break;
	    case '{': tib_start=i; break;
	    case '}': tib_end=i+1; break;
	    }
	}
    }

    /* Agora faça a formatação */
    buf[0]=0;

    for(i=0; i<fmtlen; i++) {
	if((i==tib_start) && (dt.day==-1)) {
	    /* controlador do Dia do Sto. Tiby*/
	    strcpy(bufptr, ("Dia do Santo Tiby"));
	    bufptr += strlen(bufptr);
	    i=tib_end;
	} else {
	    if(fmt[i]=='%') {
		char *wibble=0, snarf[23];
		switch(fmt[++i]) {
		case 'A': wibble=day_long[dt.yday%5]; break;
		case 'a': wibble=day_short[dt.yday%5]; break;
		case 'B': wibble=season_long[dt.season]; break;
		case 'b': wibble=season_short[dt.season]; break;
		case 'd': sprintf(snarf, "%d", dt.day+1); wibble=snarf; break;
		case 'e': sprintf(snarf, "%d%s", dt.day+1, ending(dt.day+1)); 
		    wibble=snarf; break;
		case 'H': if(dt.day==4||dt.day==49)
		    wibble=holyday[dt.season][dt.day==49]; break;
		case 'N': if(dt.day!=4&&dt.day!=49) goto eschaton; break;
		case 'n': *(bufptr++)='\n'; break;
		case 't': *(bufptr++)='\t'; break;

		case 'Y': sprintf(snarf, "%d", dt.year); wibble=snarf; break;
		case '.': wibble=sel(excl, ARRAY_SIZE(excl));
		    break;
#ifdef KILL_BOB
		case 'X': sprintf(snarf, "%d", 
				  xday_countdown(dt.yday, dt.year));
				  wibble = snarf; break;
#endif /* KILL_BOB */
		}
		if(wibble) {
/*		    fprintf(stderr, "wibble = (%s)\n", wibble);*/
		    strcpy(bufptr, wibble); bufptr+=strlen(wibble);
		}
	    } else {
		*(bufptr++) = fmt[i];
	    }
	}
    }
  eschaton:
    *(bufptr)=0;
}
/* ordem p/ o input de data*/
struct disc_time makeday(int imonth,int iday,int iyear) 
{ 
    struct disc_time funkychickens;
    
    int cal[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int dayspast=0;

    memset(&funkychickens,0,sizeof(funkychickens));
    /* checa o range básico */
    if (imonth < 1 || imonth > 12 || iyear == 0) {
	    funkychickens.season = -1;
	    return funkychickens;
    }
    if (iday < 1 || iday > cal[imonth-1]) {
	    if (!(imonth == 2 && iday == 29 && iyear%4 == 0 &&
		  (iyear%100 != 0 || iyear%400 == 0))) {
		    funkychickens.season = -1;
		    return funkychickens;
	    }
    }
    
    imonth--;
    /* Nota: No calendário gregoriano não existe o ano 0
     * então adicione um se você quer espeficicar um ano menor que o 0 */
    funkychickens.year= iyear+1166 + ((0 > iyear)?1:0);
    while(imonth>0) { dayspast+=cal[--imonth]; }
    funkychickens.day=dayspast+iday-1;
    funkychickens.season=0;
    if((funkychickens.year%4)==2) {
	if (funkychickens.day==59 && iday==29)  funkychickens.day=-1;
    }
    funkychickens.yday=funkychickens.day;
/*      nota: Sinal de igual... espero que isso resolva */
    while(funkychickens.day>=73) {
	funkychickens.season++;
	funkychickens.day-=73;
    }
    return funkychickens;
}

struct disc_time convert(int nday, int nyear)
{  struct disc_time funkychickens;
   
   funkychickens.year = nyear+3066;
   funkychickens.day=nday;
   funkychickens.season=0;
   if ((funkychickens.year%4)==2)
     {if (funkychickens.day==59)
	funkychickens.day=-1;
     else if (funkychickens.day >59)
       funkychickens.day-=1;
    }
   funkychickens.yday=funkychickens.day;
   while (funkychickens.day>=73)
     { funkychickens.season++;
       funkychickens.day-=73;
     }
   return funkychickens;
  
 }

#ifdef KILL_BOB

 /* Código para a contagem regressiva para o Dia-X, o Dia-X sendo 40 de Cfn, 3164
  * 
  * Depois que o Dia-X passou sem nenhum incidente, a IdSG declarou que o ano
  * tinha virado de cabeça para baixo, então o Dia-X seria em 8661 ao invés de
  * 1998.
  *
  * Então, o verdadeiro Dia-X é em 40 de Cnf, 9827 YOLD
  */

int xday_countdown(int yday, int year) {
    int r=(185-yday)+(((yday<59)&&(leapp(year)))?1:0);
    while(year<9827) r+=(leapp(++year)?366:365);
    while(year>9827) r-=(leapp(year--)?366:365);
    return r;
}

#endif
