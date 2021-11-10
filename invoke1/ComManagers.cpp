#include "ComManagers.h"
#include <QDebug>
ComManagers::ComManagers()
{

}

ComManagers::~ComManagers()
{

}

bool ComManagers::Conflict(std::string func_name, IComInterface* com)
{
	auto it = com_func_map_.find(func_name);
	if (it != com_func_map_.end())
	{
		for (auto cm : it->second)
		{
			if (cm->ArgEqual(com->Get_Args()))
			{
				return true;
			}
		}
	}
	else
	{
		return true;
	}
	return false;
}

bool ComManagers::Register(std::string func_name, IComInterface* com)
{
	if (Conflict(func_name, com))
	{
		return false;
	}
	auto it = com_func_map_.find(func_name);
	if (it != com_func_map_.end())
	{
		com_func_map_[func_name].push_back(com);
	}
	else
	{
		std::list<IComInterface*> lst;
		lst.push_back(com);
		com_func_map_[func_name] = lst;
	}
	return true;
}

QVariant ComManagers::Invoke(std::string req)
{
	return QVariant();
}

bool ComManagers::UnRegister(std::string func_name, QList<QPair<QVariant::Type, std::string> > params)
{
	auto it = com_func_map_.find(func_name);
	if (it != com_func_map_.end())
	{
		it->second.remove_if([params](IComInterface*& com) {return com->ArgEqual(params); });
	}
	return true;
}

QVariant ComManagers::Invoke(std::string func_name, const char* params)
{
	auto it = com_func_map_.find(func_name);
	if (it == com_func_map_.end())
	{
		return false;
	}
	else
	{
		for (auto com_inter : it->second)
		{
			if (com_inter->checkArg(params))
			{
				QJsonObject var;
				bool ret = com_inter->invoke(params, std::bind(&ComManagers::ResultHandle,this, std::placeholders::_1, std::placeholders::_2), var);
				if (ret)
				{
					QJsonValue value = var;
					return value.toVariant();
				}
			}
		}
	}
	return false;
}

void ComManagers::ResultHandle(QVariant in_var, QJsonObject& out_var)
{
	auto type = in_var.type();
	switch (type)
	{
	case QVariant::UserType:	
	{
		std::string type_name =in_var.typeName();
		if (type_name == "std::string")
		{
			out_var["ret"] = in_var.value<std::string>().c_str();
		}
	}
		break;
	case QVariant::Invalid:
		break;
	case QVariant::Bool:
	{
		out_var["ret"] = in_var.toBool();
	}
	break;
	case QVariant::Int:
	{
		out_var["ret"] = in_var.toInt();
	}
		break;
	case QVariant::UInt:
	{
		out_var["ret"] = in_var.toDouble();
	}
		break;
	case QVariant::LongLong:
	{
		out_var["ret"] = in_var.toLongLong();
	}
		break;
	case QVariant::ULongLong:
	{
		out_var["ret"] = in_var.toDouble();
	}
		break;
	case QVariant::Double:
	{
		out_var["ret"] = in_var.toDouble();
	}
		break;
	case QVariant::Map:
		break;
	case QVariant::List:
		break;
	case QVariant::String:
		break;
	case QVariant::StringList:
		break;
	case QVariant::Date:
		break;
	case QVariant::Char:
		break;
	case QVariant::Time:
		break;
	case QVariant::DateTime:
		break;
	case QVariant::Url:
		break;
	case QVariant::Locale:
		break;
	case QVariant::Rect:
		break;
	case QVariant::RectF:
		break;
	case QVariant::Size:
		break;
	case QVariant::SizeF:
		break;
	case QVariant::Line:
		break;
	case QVariant::LineF:
		break;
	case QVariant::Point:
		break;
	case QVariant::PointF:
		break;
	case QVariant::RegExp:
		break;
	case QVariant::RegularExpression:
		break;
	case QVariant::Hash:
		break;
	case QVariant::EasingCurve:
		break;
	case QVariant::Uuid:
		break;
	case QVariant::ModelIndex:
		break;
	case QVariant::PersistentModelIndex:
		break;
	case QVariant::LastCoreType:
		break;
	case QVariant::Font:
		break;
	case QVariant::Pixmap:
		break;
	case QVariant::Brush:
		break;
	case QVariant::Color:
		break;
	case QVariant::Palette:
		break;
	case QVariant::Image:
		break;
	case QVariant::Polygon:
		break;
	case QVariant::Region:
		break;
	case QVariant::Bitmap:
		break;
	case QVariant::Cursor:
		break;
	case QVariant::KeySequence:
		break;
	case QVariant::Pen:
		break;
	case QVariant::TextLength:
		break;
	case QVariant::TextFormat:
		break;
	case QVariant::Matrix:
		break;
	case QVariant::Transform:
		break;
	case QVariant::Matrix4x4:
		break;
	case QVariant::Vector2D:
		break;
	case QVariant::Vector3D:
		break;
	case QVariant::Vector4D:
		break;
	case QVariant::Quaternion:
		break;
	case QVariant::PolygonF:
		break;
	case QVariant::Icon:
		break;
	case QVariant::SizePolicy:
		break;
	case QVariant::LastType:
		break;
	default:
		break;
	}
}
