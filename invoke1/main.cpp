#include <QtCore/QCoreApplication>
#include <QMetaType>
#include <QList>
#include <memory>
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
	
	std::shared_ptr< TestFunc_int_1> func_obj_1 = std::make_shared< TestFunc_int_1>();

	QList<QPair<QVariant::Type, std::string> > params;
	params.push_back(QPair<QVariant::Type, std::string>(QVariant::Int, "test_input"));
	std::shared_ptr < IComInterface> com_interface = std::make_shared< ComMethod1<std::string, TestFunc_int_1,int>>(std::bind(&TestFunc_int_1::Func_inttostring, func_obj_1, std::placeholders::_1), func_obj_1,params);
	ComManagers::GetInstance()->Register("Func_int_1", com_interface);
	QJsonObject obj;
	obj["test_input"] = 3;
	auto pam = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
	auto ret = ComManagers::GetInstance()->Invoke("Func_int_1", pam.toLocal8Bit());
	QString r(QJsonDocument(ret.toJsonObject()).toJson(QJsonDocument::Compact));


	QList<QPair<QVariant::Type, std::string> > params1;
	params1.push_back(QPair<QVariant::Type, std::string>(QVariant::ULongLong, "test_input"));
	params1.push_back(QPair<QVariant::Type, std::string>(QVariant::ULongLong, "test_input2"));
	std::shared_ptr < IComInterface>  com_interface1 = std::make_shared < ComMethod2<std::string, TestFunc_int_1,uint64_t, uint64_t>>(std::bind(&TestFunc_int_1::Func_int2tostring, func_obj_1, std::placeholders::_1, std::placeholders::_2), func_obj_1,params1);
	ComManagers::GetInstance()->Register("Func_int_2", com_interface1);
	QJsonObject obj1;
	obj1["test_input"] = 3;
	obj1["test_input2"] = 4;
	auto pam1 = QString(QJsonDocument(obj1).toJson(QJsonDocument::Compact));
	auto ret1 = ComManagers::GetInstance()->Invoke("Func_int_2", pam1.toLocal8Bit());
	QString r1(QJsonDocument(ret1.toJsonObject()).toJson(QJsonDocument::Compact));


	
    return a.exec();
}
