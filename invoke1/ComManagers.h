#pragma once
#include "com_method.h"
#include <map>
#include <string>
#include <List>
#define OPEN_RESOURCE_REVIEW_SVR_METHOD "OpenResourceReview"
#define OPEN_REVIEWROOM_RESOURCEVERSION_SVR_METHOD "OpenReviewRoomResourceVersion"
class ComManagers
{
public:
	~ComManagers();

	static ComManagers *GetInstance()
	{
		static ComManagers notify_center;
		return &notify_center;
	}

	bool Conflict(std::string, std::shared_ptr<IComInterface>com);
	bool Register(std::string, std::shared_ptr<IComInterface> com);
	bool UnRegister(std::string, QList<QPair<QVariant::Type, std::string> > params);
	bool UnRegister(std::string, std::shared_ptr<IComInterface> com);

	QVariant Invoke(std::string req);
	QVariant Invoke(std::string funcname, const char* params);

	void ResultHandle(QVariant in_var, QJsonObject& out_var);
private:
	explicit ComManagers();
	std::map<std::string, std::list<std::shared_ptr<IComInterface>>> com_func_map_;
};
