/*
 * AuthWebForm.h
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#ifndef AUTHWEBFORM_H_
#define AUTHWEBFORM_H_

#include <FUi.h>
#include <FWeb.h>

//, public Tizen::Web::Controls::IWebKeypadEventListener

class AuthWebForm
 : public Tizen::Ui::Controls::Form
 , public Tizen::Ui::Scenes::ISceneEventListener
 , public Tizen::Ui::Controls::IFormBackEventListener
 , public Tizen::Web::Controls::ILoadingListener
 , public Tizen::Web::Controls::IWebUiEventListener

{
public:
	AuthWebForm();
	virtual ~AuthWebForm();

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
									   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
										const Tizen::Ui::Scenes::SceneId& nextSceneId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	virtual bool OnHttpAuthenticationRequestedN(const Tizen::Base::String& host, const Tizen::Base::String& realm, const Tizen::Web::Controls::AuthenticationChallenge& authentication);
	virtual void OnHttpAuthenticationCanceled(void);
	virtual void OnLoadingStarted(void);
	virtual void OnLoadingCanceled(void);
	virtual void OnLoadingErrorOccurred(Tizen::Web::Controls::LoadingErrorType error, const Tizen::Base::String& reason);
	virtual void OnLoadingCompleted(void);
	virtual void OnEstimatedProgress(int progress);
	virtual void OnPageTitleReceived(const Tizen::Base::String& title);
	virtual bool OnLoadingRequested(const Tizen::Base::String& url, Tizen::Web::Controls::WebNavigationType type);
	virtual Tizen::Web::Controls::DecisionPolicy OnWebDataReceived(const Tizen::Base::String& mime, const Tizen::Net::Http::HttpHeader& header);

	virtual void OnWebPageBlockSelected(Tizen::Web::Controls::Web &source, Tizen::Graphics::Point &startPoint, Tizen::Graphics::Point &endPoint);
	virtual void OnWebPageShowRequested(Tizen::Web::Controls::Web &source);
	virtual void OnWebWindowCloseRequested(Tizen::Web::Controls::Web &source);
	virtual Tizen::Web::Controls::Web* OnWebWindowCreateRequested(void);

private:
	Tizen::Web::Controls::Web* __pWeb;

private:
	void SetUrl(const Tizen::Base::String& url);
	bool ExtractAccessDataAndSave(const Tizen::Base::String& url);
	Tizen::Base::String* RequestAuthUrl();

	bool __watingForReqistration;
};

#endif /* AUTHWEBFORM_H_ */
