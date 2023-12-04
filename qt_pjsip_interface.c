/* $Id$ */
/*
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <pjsua-lib/pjsua.h>
#include "pjsua_app_common.h"
#include "qt_pjsip_interface.h"

#define THIS_FILE	"pjsua_app_legacy.c"


/* An attempt to avoid stdout buffering for python tests:
 * - call 'fflush(stdout)' after each call to 'printf()/puts()'
 * - apply 'setbuf(stdout, 0)', but it is not guaranteed by the standard:
 *   http://stackoverflow.com/questions/1716296
 */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    (defined (_MSC_VER) && _MSC_VER >= 1400)
/* Variadic macro is introduced in C99; MSVC supports it in since 2005. */
#  define printf(...) {printf(__VA_ARGS__);fflush(stdout);}
#  define puts(s) {puts(s);fflush(stdout);}
#endif


 pj_bool_t	cmd_echo;
 qt_ua_info  qt_ua_data;//ua share state data,account,call and conference
char glb_serverip[80];
/*
 * Print buddy list.
 */
 void print_buddy_list()
{
    pjsua_buddy_id ids[64];
    int i;
    unsigned count = PJ_ARRAY_SIZE(ids);

    puts("Buddy list:");

    pjsua_enum_buddies(ids, &count);

    if (count == 0) {
	puts(" -none-");
    } else {
	for (i=0; i<(int)count; ++i) {
	    pjsua_buddy_info info;

	    if (pjsua_buddy_get_info(ids[i], &info) != PJ_SUCCESS)
		continue;

	    printf(" [%2d] <%.*s>  %.*s\n",
		    ids[i]+1,
		    (int)info.status_text.slen,
		    info.status_text.ptr,
		    (int)info.uri.slen,
		    info.uri.ptr);
	}
    }
    puts("");
}


/*
 * Input URL.
 */
 int pjsip_input_url(char* new_url,char *buf, pj_size_t len)
{
	if(buf==NULL||new_url==NULL)
		return -1;
    int i;
	char uname[80];
	char calledURL[80];

   
  

    /* check phone num */


	for (; i<len; ++i) {
	    if (!pj_isdigit(buf[i])) {
		return -1;
	    }
	}
		
	pj_status_t status;

    sprintf(new_url,"sip:%s@%s",buf,glb_serverip);

	printf("will call party  %s",new_url);

	if ((status=pjsua_verify_url(new_url)) != PJ_SUCCESS) {
	    pjsua_perror(THIS_FILE, "Invalid URL", status);
	    return -1;
	}

	return 0;
    
}


/*
 * Print account status.
 */
 void print_acc_status(int acc_id)
{
    char buf[80];
    pjsua_acc_info info;

    pjsua_acc_get_info(acc_id, &info);

    if (!info.has_registration) {
	pj_ansi_snprintf(buf, sizeof(buf), "%.*s",
			 (int)info.status_text.slen,
			 info.status_text.ptr);

    } else {
	pj_ansi_snprintf(buf, sizeof(buf),
			 "%d/%.*s (expires=%d)",
			 info.status,
			 (int)info.status_text.slen,
			 info.status_text.ptr,
			 info.expires);

    }

    printf(" %c[%2d] %.*s: %s\n", (acc_id==current_acc?'*':' '),
	   acc_id,  (int)info.acc_uri.slen, info.acc_uri.ptr, buf);
    printf("       Online status: %.*s\n",
	(int)info.online_status_text.slen,
	info.online_status_text.ptr);
}

/*
 * Show a bit of help.
 */
 void keystroke_help()
{
    pjsua_acc_id acc_ids[16];
    unsigned count = PJ_ARRAY_SIZE(acc_ids);
    int i;

    printf(">>>>\n");

    pjsua_enum_accs(acc_ids, &count);

    printf("Account list:\n");
    for (i=0; i<(int)count; ++i)
	print_acc_status(acc_ids[i]);

    print_buddy_list();

    //puts("Commands:");
    puts("+=============================================================================+");
    puts("|       Call Commands:         |   Buddy, IM & Presence:  |     Account:      |");
    puts("|                              |                          |                   |");
    puts("|  m  Make new call            | +b  Add new buddy       .| +a  Add new accnt |");
    puts("|  M  Make multiple calls      | -b  Delete buddy         | -a  Delete accnt. |");
    puts("|  a  Answer call              |  i  Send IM              | !a  Modify accnt. |");
    puts("|  h  Hangup call  (ha=all)    |  s  Subscribe presence   | rr  (Re-)register |");
    puts("|  H  Hold call                |  u  Unsubscribe presence | ru  Unregister    |");
    puts("|  v  re-inVite (release hold) |  t  ToGgle Online status |  >  Cycle next ac.|");
    puts("|  U  send UPDATE              |  T  Set online status    |  <  Cycle prev ac.|");
    puts("| ],[ Select next/prev call    +--------------------------+-------------------+");
    puts("|  x  Xfer call                |      Media Commands:     |  Status & Config: |");
    puts("|  X  Xfer with Replaces       |                          |                   |");
    puts("|  #  Send RFC 2833 DTMF       | cl  List ports           |  d  Dump status   |");
    puts("|  *  Send DTMF with INFO      | cc  Connect port         | dd  Dump detailed |");
    puts("| dq  Dump curr. call quality  | cd  Disconnect port      | dc  Dump config   |");
    puts("|                              |  V  Adjust audio Volume  |  f  Save config   |");
    puts("|  S  Send arbitrary REQUEST   | Cp  Codec priorities     |                   |");
    puts("+-----------------------------------------------------------------------------+");
#if PJSUA_HAS_VIDEO
    puts("| Video: \"vid help\" for more info                                             |");
    puts("+-----------------------------------------------------------------------------+");
#endif
    puts("|  q  QUIT   L  ReLoad   sleep MS   echo [0|1|txt]     n: detect NAT type     |");
    puts("+=============================================================================+");

    i = pjsua_call_get_count();
    printf("You have %d active call%s\n", i, (i>1?"s":""));

    if (current_call != PJSUA_INVALID_ID) {
	pjsua_call_info ci;
	if (pjsua_call_get_info(current_call, &ci)==PJ_SUCCESS)
	    printf("Current call id=%d to %.*s [%.*s]\n", current_call,
		   (int)ci.remote_info.slen, ci.remote_info.ptr,
		   (int)ci.state_text.slen, ci.state_text.ptr);
    }
}



 void qt_make_new_call(char * calledname)
{
    char buf[128];
    pjsua_msg_data msg_data_;
    input_result result;
    pj_str_t tmp;
    if(calledname==NULL)
		return ;
	int ret;

    printf("(You currently have %d calls   %s)\n", pjsua_call_get_count(), calledname);

	ret=pjsip_input_url(buf,calledname,strlen(calledname));

    if (!ret) {
	tmp = pj_str(buf);
    } else {
	tmp.slen = 0;
    }

    pjsua_msg_data_init(&msg_data_);
    TEST_MULTIPART(&msg_data_);
    pjsua_call_make_call(current_acc, &tmp, &call_opt, NULL,
			 &msg_data_, &current_call);
}


 void qt_get_curr_callinfo( pjsua_call_info *call_info)
 {
  if(call_info==NULL)
   return ;

  qt_call_info * qt_callinfo=NULL;
  qt_callinfo=&qt_ua_data.currentcall_info;
  memset(qt_callinfo,0,sizeof(qt_callinfo));
  qt_callinfo->acc_id=call_info->acc_id;
  qt_callinfo->id=call_info->id;

  strncpy(qt_callinfo->local_info,call_info->local_info.ptr,call_info->local_info.slen);
  strncpy(qt_callinfo->remote_info,call_info->remote_info.ptr,call_info->remote_info.slen);
  qt_callinfo->state=call_info->state;
  strncpy(qt_callinfo->state_text,call_info->state_text.ptr,call_info->state_text.slen);
  qt_callinfo->last_status=call_info->last_status;
  strncpy(qt_callinfo->last_status_text,call_info->last_status_text.ptr,call_info->last_status_text.slen);
  if(call_info->media_status==PJSUA_CALL_MEDIA_REMOTE_HOLD)
   {
     qt_callinfo->hold_flg=1;
   }

  qt_ua_data.call_cnt=pjsua_call_get_count();
 }



void qt_get_reginfo()
{
    //pjsua_acc_enum_info()

};

/** UI Command **/

 void qt_make_multi_call(char * calledname)
{
  /*to be add*/
}

 void qt_detect_nat_type()
{
    int i = pjsua_detect_nat_type();
    if (i != PJ_SUCCESS)
	pjsua_perror(THIS_FILE, "Error", i);
}

 void qt_send_instant_message(char * calledname,char * msgtext)
{
    char *uri = NULL;
    /* i is for call index to send message, if any */
    int i = -1;
    char buf[128];
    char text[128];
    pj_str_t tmp;
	int ret;

	ret=pjsip_input_url(buf,calledname,strlen(calledname));

    if (!ret) {
	tmp = pj_str(buf);
    } else {
	tmp.slen = 0;
    }

    /* Input the IM . */
 
	/*
	* Cancelled.
	* Send typing notification too, saying we're not typing.
	*/
	// to be modify,use in dialog or not  fan!!!!!!!!!!!
    /* Send the IM */
    if (i != -1)
	pjsua_call_send_im(i, NULL, &tmp, NULL, NULL);
    else {
	pj_str_t tmp_uri = pj_str(uri);
	pjsua_im_send(current_acc, &tmp_uri, NULL, &tmp, NULL, NULL);
    }
}

 void qt_answer_call()
{
	
	qt_answer_call_inner(200,NULL);

		
}



 void qt_forwarding_call(char *nextphone)
{
	if(nextphone==NULL)
		return ;
	qt_answer_call_inner(302,nextphone);
}

//only in 3XX ,should have a nextphone
//the 
 void qt_answer_call_inner(int st_code,char *nextphone)
{
    pjsua_call_info call_info;
    char buf[128];
    pjsua_msg_data msg_data_;
	int ret;

    if (current_call != -1) {
	pjsua_call_get_info(current_call, &call_info);
    } else {
	/* Make compiler happy */
	call_info.role = PJSIP_ROLE_UAC;
	call_info.state = PJSIP_INV_STATE_DISCONNECTED;
    }

    if (current_call == -1 ||
	call_info.role != PJSIP_ROLE_UAS ||
	call_info.state >= PJSIP_INV_STATE_CONNECTING)
    {
	puts("No pending incoming call");
	fflush(stdout);
	return;

    } else {
	char contact[120];
	pj_str_t hname = { "Contact", 7 };
	pj_str_t hvalue;
	pjsip_generic_string_hdr hcontact;

	if (st_code < 100)
	    return;

	pjsua_msg_data_init(&msg_data_);

	if (st_code/100 == 3) {
		ret=pjsip_input_url(contact,nextphone,strlen(nextphone));
		
	    if(ret)
				return;
	    hvalue = pj_str(contact);
	    pjsip_generic_string_hdr_init2(&hcontact, &hname, &hvalue);

	    pj_list_push_back(&msg_data_.hdr_list, &hcontact);
	}

	/*
	* Must check again!
	* Call may have been disconnected while we're waiting for
	* keyboard input.
	*/
	if (current_call == -1) {
		//report status
	    //puts("Call has been disconnected");
	    //fflush(stdout);
	    return;
	}

	pjsua_call_answer2(current_call, &call_opt, st_code, NULL, &msg_data_);
    }
}

//1 hangup all  0 current call
 void qt_hangup_call(int ha)
{
    if (current_call == -1) {
	puts("No current call");
	fflush(stdout);
	return;

    } else if (ha == 1) {
	/* Hangup all calls */
	pjsua_call_hangup_all();
    } else {
	/* Hangup current calls */
	pjsua_call_hangup(current_call, 0, NULL, NULL);
    }
}



 void qt_cycle_dialog(int direct)
{
    if (direct == 1) {
	find_next_call();

    } else {
	find_prev_call();
    }

    if (current_call != -1) {
	pjsua_call_info call_info;

	pjsua_call_get_info(current_call, &call_info);
	PJ_LOG(3,(THIS_FILE,"Current dialog: %.*s",
	    (int)call_info.remote_info.slen,
	    call_info.remote_info.ptr));

    } else {
	PJ_LOG(3,(THIS_FILE,"No current dialog"));
    }
}


//index or id ???
 int  qt_cycle_account(int accid)
{
    int i;
    char buf[128];



    i = accid;
    if (pjsua_acc_is_valid(i)) {
	pjsua_acc_set_default(i);
	return 0;
	PJ_LOG(3,(THIS_FILE, "Current account changed to %d", i));
    } else {
	return -1;
	PJ_LOG(3,(THIS_FILE, "Invalid account id %d", i));
    }
}

//not used now

 void ui_add_buddy()
{
    char buf[128];
    pjsua_buddy_config buddy_cfg;
    pjsua_buddy_id buddy_id;
    pj_status_t status;


    if (pjsua_verify_url(buf) != PJ_SUCCESS) {
	printf("Invalid URI '%s'\n", buf);
	return;
    }

    pj_bzero(&buddy_cfg, sizeof(pjsua_buddy_config));

    buddy_cfg.uri = pj_str(buf);
    buddy_cfg.subscribe = PJ_TRUE;

    status = pjsua_buddy_add(&buddy_cfg, &buddy_id);
    if (status == PJ_SUCCESS) {
	printf("New buddy '%s' added at index %d\n",
	    buf, buddy_id+1);
    }
}

 void qt_add_account(char *uname,char *registrar,char *realm,char * passwd)
 {
     pjsua_transport_config *rtp_cfg=NULL;
  rtp_cfg=&app_config.rtp_cfg;
     char id[80];
     char registrarsip[80];
     memset(id,0,sizeof(id));
     memset(registrarsip,0,sizeof(registrarsip));
     memset(glb_serverip,0,sizeof(glb_serverip));
     pjsua_acc_config acc_cfg;
     pj_status_t status;
  if(uname==NULL||registrar==NULL||realm==NULL||passwd==NULL)
   return;
    sprintf(id,"sip:%s@%s",uname,registrar);
    sprintf(registrarsip,"sip:%s",registrar);
    sprintf(glb_serverip,"%s",registrar);


	printf("Your SIP URL:%s", id);
    printf("URL of the registrar:%s", registrarsip);
  	printf("Auth Realm:%s", realm);
   	printf("Auth Username:%s", uname);
 	printf("Auth Password:%s", passwd);
   
    pjsua_acc_config_default(&acc_cfg);
    acc_cfg.id = pj_str(id);
    acc_cfg.reg_uri = pj_str(registrarsip);
    acc_cfg.cred_count = 1;
    acc_cfg.cred_info[0].scheme = pjsip_DIGEST_STR;
    acc_cfg.cred_info[0].realm = pj_str(realm);
    acc_cfg.cred_info[0].username = pj_str(uname);
    acc_cfg.cred_info[0].data_type = 0;
    acc_cfg.cred_info[0].data = pj_str(passwd);

    acc_cfg.rtp_cfg = *rtp_cfg;
    app_config_init_video(&acc_cfg);

    status = pjsua_acc_add(&acc_cfg, PJ_TRUE, NULL);
    if (status != PJ_SUCCESS) {
	pjsua_perror(THIS_FILE, "Error adding new account", status);
    }
}

#if 0
//not used now
 void ui_delete_buddy()
{
    char buf[128];
    int i;

    if (!simple_input("Enter buddy ID to delete", buf, sizeof(buf)))
	return;

    i = my_atoi(buf) - 1;

    if (!pjsua_buddy_is_valid(i)) {
	printf("Invalid buddy id %d\n", i);
    } else {
	pjsua_buddy_del(i);
	printf("Buddy %d deleted\n", i);
    }
}
#endif
 void qt_delete_account(int accid)
{
    char buf[128];
    int i;
    i=accid;

    i = my_atoi(buf);

    if (!pjsua_acc_is_valid(i)) {
	printf("Invalid account id %d\n", i);
    } else {
	pjsua_acc_del(i);
	printf("Account %d deleted\n", i);
    }
}

 void qt_call_hold()
{
    if (current_call != -1) {
	pjsua_call_set_hold(current_call, NULL);
    } else {
	PJ_LOG(3,(THIS_FILE, "No current call"));
    }
}


//call resume
 void qt_call_reinvite()
{
    call_opt.flag |= PJSUA_CALL_UNHOLD;
    pjsua_call_reinvite2(current_call, &call_opt, NULL);
}


 void qt_send_update()
{
    if (current_call != -1) {
	pjsua_call_update2(current_call, &call_opt, NULL);
    } else {
	PJ_LOG(3,(THIS_FILE, "No current call"));
    }
}

/*
 * Change codec priorities.
 */
 void ui_manage_codec_prio()
{
    pjsua_codec_info c[32];
    unsigned i, count = PJ_ARRAY_SIZE(c);
    char input[32];
    char *codec, *prio;
    pj_str_t id;
    int new_prio;
    pj_status_t status;

    printf("List of audio codecs:\n");
    pjsua_enum_codecs(c, &count);
    for (i=0; i<count; ++i) {
	printf("  %d\t%.*s\n", c[i].priority, (int)c[i].codec_id.slen,
			       c[i].codec_id.ptr);
    }


    puts("");
    puts("Enter codec id and its new priority (e.g. \"speex/16000 200\", "
	 """\"H263 200\"),");
    puts("or empty to cancel.");

    printf("Codec name (\"*\" for all) and priority: ");
    if (fgets(input, sizeof(input), stdin) == NULL)
	return;
    if (input[0]=='\r' || input[0]=='\n') {
	puts("Done");
	return;
    }

    codec = strtok(input, " \t\r\n");
    prio = strtok(NULL, " \r\n");

    if (!codec || !prio) {
	puts("Invalid input");
	return;
    }

    new_prio = atoi(prio);
    if (new_prio < 0)
	new_prio = 0;
    else if (new_prio > PJMEDIA_CODEC_PRIO_HIGHEST)
	new_prio = PJMEDIA_CODEC_PRIO_HIGHEST;

    status = pjsua_codec_set_priority(pj_cstr(&id, codec),
				      (pj_uint8_t)new_prio);
    if (status != PJ_SUCCESS)
	pjsua_perror(THIS_FILE, "Error setting codec priority", status);
}



 void qt_call_transfer(pj_bool_t no_refersub,char *calledname)
{
    if (current_call == -1) {
	PJ_LOG(3,(THIS_FILE, "No current call"));
    } else 
	{
		int call = current_call;
		char buf[128];
		int ret;
		pjsip_generic_string_hdr refer_sub;
		pj_str_t STR_REFER_SUB = { "Refer-Sub", 9 };
		pj_str_t STR_FALSE = { "false", 5 };
		pjsua_call_info ci;
		input_result result;
		pjsua_msg_data msg_data_;

		pjsua_call_get_info(current_call, &ci);
		printf("Transferring current call [%d] %.*s\n", current_call,
		       (int)ci.remote_info.slen, ci.remote_info.ptr);

		ret=pjsip_input_url(buf,calledname,strlen(calledname));

	  

		/* Check if call is still there. */

		if (call != current_call) {
		    puts("Call has been disconnected");
		    return;
		}

		pjsua_msg_data_init(&msg_data_);
		if (no_refersub) {
		    /* Add Refer-Sub: false in outgoing REFER request */
		    pjsip_generic_string_hdr_init2(&refer_sub, &STR_REFER_SUB,
			&STR_FALSE);
		    pj_list_push_back(&msg_data_.hdr_list, &refer_sub);
		} 
		   pj_str_t tmp;
		  if (!ret){
					tmp = pj_str(buf);
	    			} else {
						tmp.slen = 0;
	    				}
		   
		
		    pjsua_call_xfer( current_call, &tmp, &msg_data_);
	}
}
#if 0
 void ui_call_transfer_replaces(pj_bool_t no_refersub)
{
    if (current_call == -1) {
	PJ_LOG(3,(THIS_FILE, "No current call"));
    } else {
	int call = current_call;
	int dst_call;
	pjsip_generic_string_hdr refer_sub;
	pj_str_t STR_REFER_SUB = { "Refer-Sub", 9 };
	pj_str_t STR_FALSE = { "false", 5 };
	pjsua_call_id ids[PJSUA_MAX_CALLS];
	pjsua_call_info ci;
	pjsua_msg_data msg_data_;
	char buf[128];
	unsigned i, count;

	count = PJ_ARRAY_SIZE(ids);
	pjsua_enum_calls(ids, &count);

	if (count <= 1) {
	    puts("There are no other calls");
	    return;
	}

	pjsua_call_get_info(current_call, &ci);
	printf("Transfer call [%d] %.*s to one of the following:\n",
	       current_call,
	       (int)ci.remote_info.slen, ci.remote_info.ptr);

	for (i=0; i<count; ++i) {
	    pjsua_call_info call_info;

	    if (ids[i] == call)
		continue;

	    pjsua_call_get_info(ids[i], &call_info);
	    printf("%d  %.*s [%.*s]\n",
		ids[i],
		(int)call_info.remote_info.slen,
		call_info.remote_info.ptr,
		(int)call_info.state_text.slen,
		call_info.state_text.ptr);
	}

	if (!simple_input("Enter call number to be replaced", buf, sizeof(buf)))
	    return;

	dst_call = my_atoi(buf);

	/* Check if call is still there. */

	if (call != current_call) {
	    puts("Call has been disconnected");
	    return;
	}

	/* Check that destination call is valid. */
	if (dst_call == call) {
	    puts("Destination call number must not be the same "
		"as the call being transferred");
	    return;
	}
	if (dst_call >= PJSUA_MAX_CALLS) {
	    puts("Invalid destination call number");
	    return;
	}
	if (!pjsua_call_is_active(dst_call)) {
	    puts("Invalid destination call number");
	    return;
	}

	pjsua_msg_data_init(&msg_data_);
	if (no_refersub) {
	    /* Add Refer-Sub: false in outgoing REFER request */
	    pjsip_generic_string_hdr_init2(&refer_sub, &STR_REFER_SUB,
					   &STR_FALSE);
	    pj_list_push_back(&msg_data_.hdr_list, &refer_sub);
	}

	pjsua_call_xfer_replaces(call, dst_call,
				 PJSUA_XFER_NO_REQUIRE_REPLACES,
				 &msg_data_);
    }
}
#endif
 void qt_send_dtmf_2833(char dtnf)
{
    if (current_call == -1) {
	PJ_LOG(3,(THIS_FILE, "No current call"));
    } else if (!pjsua_call_has_media(current_call)) {
	PJ_LOG(3,(THIS_FILE, "Media is not established yet!"));
    } else {
	pj_str_t digits;
	int call = current_call;
	pj_status_t status;
	char buf[128];
	if (call != current_call) {
	    puts("Call has been disconnected");
	    return;
	}

	digits = pj_str(dtnf);
	status = pjsua_call_dial_dtmf(current_call, &digits);
	if (status != PJ_SUCCESS) {
	    pjsua_perror(THIS_FILE, "Unable to send DTMF", status);
	} else {
	    puts("DTMF digits enqueued for transmission");
	}
    }
}

 void qt_send_dtmf_info(char dtnf)
{
    if (current_call == -1) {
	PJ_LOG(3,(THIS_FILE, "No current call"));
    } else {
	int call = current_call;
	pj_status_t status;
	char buf[128];
	pjsua_call_send_dtmf_param param;

	if (call != current_call) {
	    puts("Call has been disconnected");
	    return;
	}	
	pjsua_call_send_dtmf_param_default(&param);
	param.digits = pj_str(dtnf);
	param.method = PJSUA_DTMF_METHOD_SIP_INFO;
	status = pjsua_call_send_dtmf(current_call, &param);
	if (status != PJ_SUCCESS) {
	    pjsua_perror(THIS_FILE, "Error sending DTMF", status);
	}
    }
}

 void qt_send_arbitrary_request( char *text,char *buf)
{
   
    char *uri;
   
    pj_str_t tmp;
	if(text==NULL)
		{
			return ;
		}

    if (pjsua_acc_get_count() == 0) {
	puts("Sorry, need at least one account configured");
	return;
    }

    puts("Send arbitrary request to remote host");
	

    if (uri) {
	tmp = pj_str(uri);
	send_request(text, &tmp);
    } else {
	/* If you send call control request using this method
	* (such requests includes BYE, CANCEL, etc.), it will
	* not go well with the call state, so don't do it
	* unless it's for testing.
	*/
	pj_str_t method = pj_str(text);
	pjsua_call_send_request(current_call, &method, NULL);
    }
}

 void qt_echo(int echoflg)
{

	cmd_echo = echoflg;

}

 void qt_sleep(int delay)
{
  
	if (delay < 0) delay = 0;
	pj_thread_sleep(delay);

}

//use for  diable  accout reg or enable reg
 void qt_set_register(int regflg)
{
    switch (regflg) {
    case 1:
	/*
	* Re-Register.
	*/
	pjsua_acc_set_registration(current_acc, PJ_TRUE);
	break;
    case 0:
	/*
	* Unregister
	*/
	pjsua_acc_set_registration(current_acc, PJ_FALSE);
	break;
    }
}

//add a new one
 void qt_conference_add_third_party(char * calledname)
{
	qt_call_hold();
	
	qt_make_new_call(calledname);
	
}

//all is come ,start conference media
 void qt_start_three_party_conference()
{
	//
	qt_cycle_dialog(-1);

	//confirm the call is holding 

	//fan 

	qt_call_reinvite();
}

/*
 * connect the ports in conference bridge,for meidia only！！！
 */
#if 0
 int qt_conf_connect(int src_port, int dst_port,int conordis)
{
   
    pj_status_t status;
    int cnt;

    if (cnt != 3)
	qt_conf_list();

	
    if (conordis) {
	status = pjsua_conf_connect(src_port, dst_port);
    } else {
	status = pjsua_conf_disconnect(src_port, dst_port);
    }
    return status;
}
#endif
 void qt_adjust_volume(int direct)
{
    char buf[128];
    char text[128];
	
    sprintf(buf, "Adjust mic level: [%4.1fx] ", app_config.mic_level);
    sprintf(buf, "Adjust speaker level: [%4.1fx] ", app_config.speaker_level);

	if(direct >0)
		{
			app_config.mic_level +=0.5;
			app_config.speaker_level +=0.5;
		}else
			{
				app_config.mic_level -=0.5;
			app_config.speaker_level -=0.5;
			}

	
	pjsua_conf_adjust_rx_level(0, app_config.mic_level);
	pjsua_conf_adjust_tx_level(0, app_config.speaker_level);

}



 void qt_dump_call_quality()
{
    if (current_call != PJSUA_INVALID_ID) {
	log_call_dump(current_call);
    } else {
	PJ_LOG(3,(THIS_FILE, "No current call"));
    }
}

 void qt_dump_configuration()
{
    char settings[2000];
    int len;

    len = write_settings(&app_config, settings, sizeof(settings));
    if (len < 1)
	PJ_LOG(1,(THIS_FILE, "Error: not enough buffer"));
    else
	PJ_LOG(3,(THIS_FILE, "Dumping configuration (%d bytes):\n%s\n",
		  len, settings));
}

 void qt_write_settings(const char *filename)
{
    char settings[2000];
    int len;

    len = write_settings(&app_config, settings, sizeof(settings));
    if (len < 1)
	PJ_LOG(1,(THIS_FILE, "Error: not enough buffer"));
    else {
	pj_oshandle_t fd;
	pj_status_t status;

	status = pj_file_open(app_config.pool, filename, PJ_O_WRONLY, &fd);
	if (status != PJ_SUCCESS) {
	    pjsua_perror(THIS_FILE, "Unable to open file", status);
	} else {
	    pj_ssize_t size = len;
	    pj_file_write(fd, settings, &size);
	    pj_file_close(fd);

	    printf("Settings successfully written to '%s'\n", filename);
	}
    }
}

/*
 * Dump application states.
 */
 void qt_app_dump(pj_bool_t detail)
{
    pjsua_dump(detail);
}

 void qt_call_redirect(int opt_redirect)
{
    if (current_call == PJSUA_INVALID_ID) {
	PJ_LOG(3,(THIS_FILE, "No current call"));
    } else {
	if (!pjsua_call_is_active(current_call)) {
	    PJ_LOG(1,(THIS_FILE, "Call %d has gone", current_call));
	} else if (opt_redirect==2) {
	    pjsua_call_process_redirect(current_call,
		PJSIP_REDIRECT_ACCEPT_REPLACE);
	} else if (opt_redirect==1) {
	    pjsua_call_process_redirect(current_call,
		PJSIP_REDIRECT_ACCEPT);
	} else if (opt_redirect==0) {
	    pjsua_call_process_redirect(current_call,
		PJSIP_REDIRECT_REJECT);
	} else {
	    pjsua_call_process_redirect(current_call,
		PJSIP_REDIRECT_STOP);
	}
    }
}

 void qt_handle_ip_change()
{
    pjsua_ip_change_param param;
    pjsua_ip_change_param_default(&param);
    pjsua_handle_ip_change(&param);
}

