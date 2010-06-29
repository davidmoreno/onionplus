/**
		onion+ - web small modular server.
		Copyright (C) 2007-2010 David Moreno Montero

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU Affero General Public License as
		published by the Free Software Foundation, either version 3 of the
		License, or (at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU Affero General Public License for more details.

		You should have received a copy of the GNU Affero General Public License
		along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <security/pam_appl.h>
#include <security/pam_misc.h>

#include "../file.h"

#include "authpam.h"
#include <response.h>

using namespace Onion;

Module *authPAMConstructor(const QStringList &l,const QMap<QString,QString> &m){
	if (l.count()!=2 || m.count()>0)
		return NULL;
	
	return new AuthPAM(l[0],l[1]);
}

/**
 * @short Creates the auth mode based on PAM
 */
AuthPAM::AuthPAM(const QString &realm, const QString &pamname)
			  : Module("AuthPAM"), realm(realm), pamname(pamname){
}

QIODevice *AuthPAM::process(Request &req, Response &res){
	QString authorization=req.getHeader("authorization");
	//DEBUG("Authorization: %s",authorization.toAscii().constData());
	bool authorized=false;
	if (!authorization.isEmpty()){
		if (!authorization.startsWith("Basic")){
			WARNING("Do not know how to deal with auth != Basic. Asked for %s",authorization.section(" ",0,0).toAscii().constData());
		}
		else{
			QString namepassword=QByteArray::fromBase64(authorization.section(" ",1).toAscii());
			QString name=namepassword.section(":",0,0);
			QString password=namepassword.section(":",1);
			authorized=authorize(name,password);
		}
	}
	if (!authorized){
		res.setHeader("WWW-Authenticate",QString("Basic realm=\"%1\"").arg(realm));
		res.setStatus(HTTP_UNAUTHORIZED);
		File *f=new File(":unauthorized.html", req, res);
		return f;
	}
	
	return nextModule ? nextModule->process(req,res) : NULL;
}

/// simple answer to the password question, needed by pam
static int authPAM_passwd(int num_msg, const struct pam_message **msg,
                struct pam_response **resp, void *appdata_ptr){
	//DEBUG("num messages %d",num_msg);
	//DEBUG("Question %s",(*msg)[0].msg);
	pam_response *r;
	
	*resp=r=(pam_response*)calloc(num_msg,sizeof(struct pam_response));
	if (r==NULL)
		return PAM_BUF_ERR;
	
	for (int i=0;i<num_msg;i++){
		r->resp=strdup((const char*)appdata_ptr);
		r->resp_retcode=0;
		r++;
	}

	//DEBUG("Done");
	return PAM_SUCCESS;
}


/**
 * @short Do the real authorization. Checks if access allowed
 */
bool AuthPAM::authorize(const QString &username, const QString &password){
	int ok;
	pam_handle_t *pamh=NULL;
	char *password_local=(char*)alloca(password.length());
	strcpy(password_local, password.toUtf8().constData());
	struct pam_conv conv = {
    authPAM_passwd,
    (void*)password_local
	};
	//DEBUG("Password %s",password_local);
	
	DEBUG("Auth on service '%s', username '%s'",pamname.toAscii().constData(), username.toUtf8().constData());
	
	ok=pam_start(pamname.toAscii().constData(), username.toUtf8().constData(),
							 &conv, &pamh);
	if (ok!=PAM_SUCCESS){
		WARNING("Error initializing PAM");
		perror("PAM");
		return false;
	}
	ok = pam_authenticate(pamh, 0);    /* is user really user? */
	if (ok!=PAM_SUCCESS){
		DEBUG("Not an user. Auth failed.");
	}
	else
		ok = pam_acct_mgmt(pamh, 0);       /* permitted access? */
	
	pam_end(pamh, ok);
	if (ok==PAM_SUCCESS){
		DEBUG("Authenticated user '%s'", username.toUtf8().constData());
		return true;
	}
	DEBUG("NOT authenticated user '%s'", username.toUtf8().constData());
	return false;
}
