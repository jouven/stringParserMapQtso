#include "stringReplacer.hpp"

#include <QDateTime>

const QMap<QString, stringReplacer_c::replaceType_ec> stringReplacer_c::strToReplaceTypeMap_sta_con(
{
//Keys are lower case because this way when reading from json we can "lower" w/e is
//there and compare, allowing to ignore case
    {	"string", stringReplacer_c::replaceType_ec::string}
    , {	"currentdatetimestring", stringReplacer_c::replaceType_ec::currentDatetimeString}
});

const std::unordered_map<stringReplacer_c::replaceType_ec, QString> stringReplacer_c::replaceTypeToStrUMap_sta_con(
{
    {	stringReplacer_c::replaceType_ec::string, "string" }
    , {	stringReplacer_c::replaceType_ec::currentDatetimeString, "currentDatetimeString" }
});

QString stringReplacer_c::replaceValueOrFormat_f() const
{
    return replaceValueOrFormat_pri;
}

void stringReplacer_c::setReplaceValueOrFormat_f(const QString& replaceValueOrFormat_par_con)
{
    replaceValueOrFormat_pri = replaceValueOrFormat_par_con;
}

stringReplacer_c::replaceType_ec stringReplacer_c::replaceType_f() const
{
    return replaceType_pri;
}

bool stringReplacer_c::useUTC_f() const
{
    return useUTC_pri;
}

void stringReplacer_c::setUseUTC_f(const bool useUTC_par_con)
{
    useUTC_pri = useUTC_par_con;
}

void stringReplacer_c::derivedExecute_f(QString* string_par)
{
    while (true)
    {
        if (replaceType_pri == replaceType_ec::string)
        {
            if (replaceValueOrFormat_pri.isNull())
            {
                appendError_f("Replace value string is null");
                break;
            }
            string_par->replace(stringTrigger_f(), replaceValueOrFormat_pri);
            break;
        }
        if (replaceType_pri == replaceType_ec::currentDatetimeString)
        {
            const QString formatedDateTimeStrTmp(useUTC_pri ?
                                            QDateTime::currentDateTimeUtc().toString(replaceValueOrFormat_pri)
                                          : QDateTime::currentDateTime().toString(replaceValueOrFormat_pri));
            string_par->replace(stringTrigger_f(), formatedDateTimeStrTmp);
            break;
        }

        appendError_f("Wrong/Uninitialized replace type");
        break;
    }
}

void stringReplacer_c::derivedStop_f()
{
    //if in the future I use some kind of low level algorithm to replace that can be interrupted...
}

void stringReplacer_c::derivedWrite_f(QJsonObject& json_ref_par) const
{
    json_ref_par["replaceValueOrFormat"] = replaceValueOrFormat_pri;
    json_ref_par["replaceType"] = replaceTypeToStrUMap_sta_con.at(replaceType_pri);
    json_ref_par["useUTC"] = useUTC_pri;
}

void stringReplacer_c::derivedRead_f(const QJsonObject& json_par_con)
{
    replaceValueOrFormat_pri = json_par_con["replaceValueOrFormat"].toString();
    replaceType_pri = strToReplaceTypeMap_sta_con.value(json_par_con["replaceType"].toString());
    useUTC_pri = json_par_con["useUTC"].toBool();
}

QString stringReplacer_c::derivedValueFormat_f()
{
    QString valueFormatResultTmp;
    while (true)
    {
        if (replaceType_pri == stringReplacer_c::replaceType_ec::empty)
        {
            appendError_f("Wrong/Uninitialized replace type");
            break;
        }
        if (replaceValueOrFormat_pri.isNull())
        {
            appendError_f("Replace value/format string is null");
            break;
        }
        valueFormatResultTmp = replaceValueOrFormat_pri;
        break;
    }
    return valueFormatResultTmp;
}

stringReplacer_c* stringReplacer_c::derivedClone_f() const
{
    stringReplacer_c* copyPtr(new stringReplacer_c);
    copyPtr->replaceType_pri = this->replaceType_pri;
    copyPtr->replaceValueOrFormat_pri = this->replaceValueOrFormat_pri;
    copyPtr->useUTC_pri  = this->useUTC_pri;
    //this could be ""optimized"" only copying the relevant fields depending on the replace type
    return copyPtr;
}

stringReplacer_c::stringReplacer_c(
        const QString& triggerString_par_con
        , const stringReplacer_c::replaceType_ec replaceType_par_con
        , const QString& replaceValueOrFormat_par_con)
    : parserBase_c(parserBase_c::type_ec::stringReplace, triggerString_par_con)
    , replaceType_pri(replaceType_par_con)
    , replaceValueOrFormat_pri(replaceValueOrFormat_par_con)
{}
