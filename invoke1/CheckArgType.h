#pragma once
#define CHECK_ARG(json_request, params, index, flag)\
do\
{\
	if(index>=params.size())\
	{\
		flag = false;\
		break;\
	}\
	QPair<QVariant::Type, std::string> param= params.at(index);\
	QVariant::Type param_type = param.first;\
	std::string param_name = param.second;\
	if(!json_request.contains(QString::fromStdString(param_name)))\
	{\
		flag = false; \
		break; \
	}\
	switch(param_type)\
	{\
		case QVariant::Bool:\
		{\
			if(json_request[param_name.c_str()].isBool())\
			{\
				flag = true; \
				break; \
			}\
		}\
		break;\
		case QVariant::Int:\
		case QVariant::UInt:\
		case QVariant::LongLong:\
		case QVariant::ULongLong:\
		case QVariant::Double:\
		{\
			if (json_request[param_name.c_str()].isDouble())\
			{\
				flag = true;\
				break;\
			}\
		}\
		break;\
		case QVariant::String:\
		{\
			if (json_request[param_name.c_str()].isString())\
			{\
				flag = true; \
				break; \
			}\
		}\
		break;\
	default:\
		flag = false;\
		break;\
	}\
}while(0);\
 

#define GET_ARG(val,json_request, params, index)\
do\
{\
	if(index>=params.size())\
	{\
		break;\
	}\
	QPair<QVariant::Type, std::string> param= params.at(index);\
	QVariant::Type param_type = param.first;\
	std::string param_name = param.second.toString();\
	if(!json_request.contains(QString::fromStdString(param_name)))\
		break;\
	switch(param_type)\
	{\
		case QVariant::Bool:\
		{\
			val = Q_ARG(bool,json_request[param_name].toBool());\
		}\
		break;\
		case QVariant::Int:\
		{\
			val = Q_ARG(int,json_request[param_name].toInt());\
		}\
		break;\
		case QVariant::UInt:\
		{\
			val = Q_ARG(uint,json_request[param_name].toDouble());\
		}\
		break;\
		case QVariant::LongLong:\
		{\
			val = Q_ARG(qlonglong,json_request[param_name].toDouble());\
		}\
		break;\
		case QVariant::ULongLong:\
		{\
			val = Q_ARG(qulonglong,json_request[param_name].toDouble());\
		}\
		break;\
		case QVariant::Double:\
		{\
			val = Q_ARG(double,json_request[param_name].toDouble());\
		}\
		break;\
		case QVariant::String:\
		{\
			val = Q_ARG(QString,json_request[param_name].toString());\
		}\
		break;\
	default:\
		break;\
	}\
}while(0);