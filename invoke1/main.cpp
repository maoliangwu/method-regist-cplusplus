#include <QtCore/QCoreApplication>
#include <QMetaType>
#include "ComManagers.h"
class TestFunc_int_1
{
public:
	TestFunc_int_1()
	{
	};
	std::string Func_inttostring(int a)
	{
		return "TestFunc_int_1::Func_inttostring";
	}
	double Func_inttodouble(int a)
	{
		return 1.2;
	}
	std::string Func_int2tostring(int a, int b)
	{
		return "TestFunc_int_1::Func_int2tostring";
	}
	std::string Func_str2tostring(QString a, int b)
	{
		return "TestFunc_int_1::Func_str2tostring";
	}
};
class TestFunc_int_3
{
public:
	TestFunc_int_3()
	{
	};
	std::string Func_3param(int b, std::string a, bool c)
	{
		c = true;
		return "TestFunc_int_3::Func_3param";
	}
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv); 
	qRegisterMetaType<std::string>("std::string");
	int a1 = qMetaTypeId<std::string>();
	int b = qMetaTypeId<QString>();
	ComManagers com_manager;
	TestFunc_int_1 func_obj_1;

	QList<QPair<QVariant::Type, std::string> > params;
	params.push_back(QPair<QVariant::Type, std::string>(QVariant::Int, "test_input"));
	IComInterface* com_interface = new  ComMethod1<std::string, int>(std::bind(&TestFunc_int_1::Func_inttostring, &func_obj_1, std::placeholders::_1), params);
	com_manager.Register("Func_int_1", com_interface);
	QJsonObject obj;
	obj["test_input"] = 3;
	auto pam = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
	auto ret = com_manager.Invoke("Func_int_1", pam.toLocal8Bit());
	QString r(QJsonDocument(ret.toJsonObject()).toJson(QJsonDocument::Compact));


	QList<QPair<QVariant::Type, std::string> > params1;
	params1.push_back(QPair<QVariant::Type, std::string>(QVariant::ULongLong, "test_input"));
	params1.push_back(QPair<QVariant::Type, std::string>(QVariant::ULongLong, "test_input2"));
	IComInterface* com_interface1 = new ComMethod2<std::string, uint64_t, uint64_t>(std::bind(&TestFunc_int_1::Func_int2tostring, &func_obj_1, std::placeholders::_1, std::placeholders::_2), params1);
	com_manager.Register("Func_int_2", com_interface1);
	QJsonObject obj1;
	obj1["test_input"] = 3;
	obj1["test_input2"] = 4;
	auto pam1 = QString(QJsonDocument(obj1).toJson(QJsonDocument::Compact));
	auto ret1 = com_manager.Invoke("Func_int_2", pam1.toLocal8Bit());
	QString r1(QJsonDocument(ret1.toJsonObject()).toJson(QJsonDocument::Compact));


	TestFunc_int_3 funcobj3;
	QList<QPair<QVariant::Type, std::string> > params3;
	params3.push_back(QPair<QVariant::Type, std::string>(QVariant::Int, "test_input"));
	params3.push_back(QPair<QVariant::Type, std::string>(QVariant::String, "test_input2"));
	params3.push_back(QPair<QVariant::Type, std::string>(QVariant::Bool, "test_input3"));
	IComInterface* com_interface3 = new  ComMethod3<std::string,int, std::string, bool>(std::bind(&TestFunc_int_3::Func_3param, &funcobj3, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), params3);
	com_manager.Register("Func_int_3", com_interface3);
	QJsonObject obj3;
	obj3["test_input"] = 3;
	obj3["test_input2"] = "sssssss";
	obj3["test_input3"] = false;
	auto pam3 = QString(QJsonDocument(obj3).toJson(QJsonDocument::Compact));
	auto ret3 = com_manager.Invoke("Func_int_3", pam3.toLocal8Bit());
	QString r3(QJsonDocument(ret3.toJsonObject()).toJson(QJsonDocument::Compact));


	QList<QPair<QVariant::Type, std::string> > params4;
	params4.push_back(QPair<QVariant::Type, std::string>(QVariant::String, "test_input"));
	params4.push_back(QPair<QVariant::Type, std::string>(QVariant::Int, "test_input2"));
	{
		IComInterface* com_interface4 = new  ComMethod2<std::string, QString, int>(std::bind(&TestFunc_int_1::Func_str2tostring, &func_obj_1, std::placeholders::_1, std::placeholders::_2), params4);
		com_manager.Register("Func_int_4", com_interface4);
		QJsonObject obj4;
		obj4["test_input"] = "Func_str2tostring";
		obj4["test_input2"] = 4;
		auto pam4 = QString(QJsonDocument(obj4).toJson(QJsonDocument::Compact));
		auto ret4 = com_manager.Invoke("Func_int_4", pam4.toLocal8Bit());
		QString r4(QJsonDocument(ret4.toJsonObject()).toJson(QJsonDocument::Compact));
	}
    return a.exec();
}
