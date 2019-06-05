#ifndef STRINGPARSERMAPQTSO_STRINGREPLACER_HPP
#define STRINGPARSERMAPQTSO_STRINGREPLACER_HPP

#include "../crossPlatformMacros.hpp"

#include "base.hpp"

#include <QMap>

#include <unordered_map>

class EXPIMP_STRINGPARSERMAPQTSO stringReplacer_c : public parserBase_c
{
    Q_OBJECT

public:
    enum class replaceType_ec
    {
        empty
        , string
        , currentDatetimeString
    };
private:
    replaceType_ec replaceType_pri = replaceType_ec::empty;
    //something like "yyyy-MM-dd hh:mm:ss.zzz"
    QString replaceValueOrFormat_pri;
    //false = localtime
    bool useUTC_pri = false;
//protected:
    void derivedExecute_f(QString* string_par) override;
    //does nothing
    void derivedStop_f() override;

    void derivedWrite_f(QJsonObject &json_ref_par) const override;
    void derivedRead_f(const QJsonObject &json_par_con) override;

    QString derivedValueFormat_f() override;
    stringReplacer_c* derivedClone_f() const override;
public:
    //required to read from json
    stringReplacer_c() = default;
    explicit stringReplacer_c(//what to replace
        const QString& triggerString_par_con
            //with what type
        , const replaceType_ec replaceType_ec
            //for the basic "replace string with string"
            //since default QString has is isnull = true, that can be checked to not replace anything
        , const QString& replaceValueOrFormat_par_con = QString()
        , const bool useUTC_par_con = false
    );

    static EXPIMP_STRINGPARSERMAPQTSO const QMap<QString, replaceType_ec> strToReplaceTypeMap_sta_con;
    static EXPIMP_STRINGPARSERMAPQTSO const std::unordered_map<replaceType_ec, QString> replaceTypeToStrUMap_sta_con;

    QString replaceValueOrFormat_f() const;
    void setReplaceValueOrFormat_f(const QString& replaceValueOrFormat_par_con);
    replaceType_ec replaceType_f() const;

    //only for currentDatetimeString replacetype
    bool useUTC_f() const;
    void setUseUTC_f(const bool useUTC_par_con);
};

#endif // STRINGPARSERMAPQTSO_STRINGREPLACER_HPP
