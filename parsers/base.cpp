#include "base.hpp"

const QMap<QString, parserBase_c::type_ec> parserBase_c::strToTypeMap_sta_con(
{
//Keys are lower case because this way when reading from json we can "lower" w/e is
//there and compare, allowing to ignore case
    {	"stringreplace", parserBase_c::type_ec::stringReplace}
    //, {	"currentdatetimestring", parserBase_c::type_ec::currentDatetimeString}
});

const std::unordered_map<parserBase_c::type_ec, QString> parserBase_c::typeToStrUMap_sta_con(
{
    {	parserBase_c::type_ec::stringReplace, "stringReplace" }
    //, {	parserBase_c::type_ec::currentDatetimeString, "currentDatetimeString" }
});

QString parserBase_c::stringTrigger_f() const
{
    return stringTrigger_pri;
}

QString parserBase_c::valueFormat_f()
{
    return derivedValueFormat_f();
}

bool parserBase_c::enabled_f() const
{
    return enabled_pri;
}

void parserBase_c::setEnabled_f(const bool enabled_par_con)
{
    enabled_pri = enabled_par_con;
}

void parserBase_c::write_f(QJsonObject& json_ref_par) const
{
    json_ref_par["type"] = typeToStrUMap_sta_con.at(type_pri);
    json_ref_par["stringTrigger"] = stringTrigger_pri;
    json_ref_par["enabled"] = enabled_pri;

    derivedWrite_f(json_ref_par);
}

void parserBase_c::read_f(const QJsonObject& json_par_con)
{
    stringTrigger_pri = json_par_con["stringTrigger"].toString();
    enabled_pri = json_par_con["enabled"].toBool();
    type_pri = strToTypeMap_sta_con.value(json_par_con["type"].toString().toLower());
    derivedRead_f(json_par_con);
}

parserBase_c::type_ec parserBase_c::type_f() const
{
    return type_pri;
}

parserBase_c*parserBase_c::clone_f(const bool inheritSameParent_par_con) const
{
    parserBase_c* cloneTmp(derivedClone_f());
    if (inheritSameParent_par_con)
    {
        cloneTmp->setParent(this->parent());
    }
    return cloneTmp;
}

parserBase_c::parserBase_c(
        const type_ec type_par_con
        , const QString& stringTrigger_par_con)
    :
      type_pri(type_par_con)
      , stringTrigger_pri(stringTrigger_par_con)
{
}

void parserBase_c::execute_f(QString* string_par)
{
    while (true)
    {
        if (string_par == nullptr)
        {
            appendError_f("String pointer is null");
            break;
        }

        if (string_par->isNull())
        {
            appendError_f("String to parse is null");
            break;
        }

        derivedExecute_f(string_par);
        break;
    }
}
