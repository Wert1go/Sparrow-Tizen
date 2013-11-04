/*
 * SettingsForm.h
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#ifndef SETTINGSFORM_H_
#define SETTINGSFORM_H_

#include <FUi.h>
#include <FNet.h>

class SettingsForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public Tizen::Net::Http::IHttpTransactionEventListener
   {
public:
	SettingsForm();
	virtual ~SettingsForm();

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	void SendRequest(void);

	// IHttpTransactionEventListener
	virtual void OnTransactionReadyToRead(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	virtual void OnTransactionAborted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, result r);
	virtual void OnTransactionReadyToWrite(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	virtual void OnTransactionHeaderCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool authRequired);
	virtual void OnTransactionCompleted(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction);
	virtual void OnTransactionCertVerificationRequiredN(Tizen::Net::Http::HttpSession& httpSession, Tizen::Net::Http::HttpTransaction& httpTransaction, Tizen::Base::String* pCert);

private:
	Tizen::Net::Http::HttpSession* __pHttpSession;
};

#endif /* SETTINGSFORM_H_ */
