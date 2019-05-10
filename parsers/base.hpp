#ifndef STRINGPARSERMAPQTSO_PARSERBASE_HPP
#define STRINGPARSERMAPQTSO_PARSERBASE_HPP

#include "../crossPlatformMacros.hpp"

#include "baseClassQtso/baseClassQt.hpp"

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QMap>

#include <unordered_map>

//QObject right now it's inherited just because it's easier to manage QObjects instead of plain "new class C/C++ objects", for the:
//setParent, deleteLater...
class EXPIMP_STRINGPARSERMAPQTSO parserBase_c : public QObject, public baseClassQt_c
{
    Q_OBJECT

public:
    enum class type_ec
    {
        base = 0
        , stringReplace = 1
    };
//    enum class executionState_ec
//    {
//        initial = 0
//        , running = 1
//        , stopped = 2
//        , finished = 3
//    };
private:
    parserBase_c::type_ec type_pri = type_ec::base;
    bool enabled_pri = true;
    //string which triggers this parser
    //this might/will change, since string size can be checked too
    //aka do something if string is certain length,
    //check if string matches a regex (there is no string trigger involved)
    QString stringTrigger_pri;
    //bool executionError_pri = false;
private Q_SLOTS:
    virtual void derivedExecute_f(QString* string_par) = 0;
    //not all derived might implement this one
    virtual void derivedStop_f() = 0;

    virtual void derivedWrite_f(QJsonObject &json_ref_par) const = 0;
    virtual void derivedRead_f(const QJsonObject &json_par_con) = 0;

    virtual QString derivedValueFormat_f() = 0;

    virtual parserBase_c* derivedClone_f() const = 0;
    //void setExecutionError_f();
protected:
    //for the derived classes to create an empty object to read from json
    parserBase_c() = default;
    parserBase_c(
            const type_ec type_par_con
            , const QString& stringTrigger_par_con
            //FUTURE regex?
    );
public:

    //use anyError and getError_f to get possible error
    void execute_f(QString* string_par);

    void stop_f();

    static EXPIMP_STRINGPARSERMAPQTSO const QMap<QString, type_ec> strToTypeMap_sta_con;
    static EXPIMP_STRINGPARSERMAPQTSO const std::unordered_map<type_ec, QString> typeToStrUMap_sta_con;

    QString stringTrigger_f() const;
    //returns the value, format or some kind of representation of the end result of the parsing
    //a meta string to show/describe, along with the type, how a keyString will be replaced
    //like execute_f it can produce an error (string) if stuff is not set properly
    QString valueFormat_f();

    bool enabled_f() const;
    void setEnabled_f(const bool enabled_par_con);

    void write_f(QJsonObject &json_ref_par) const;
    void read_f(const QJsonObject &json_par_con);

    parserBase_c::type_ec type_f() const;

    parserBase_c* clone_f(const bool inheritSameParent_par_con = true) const;

Q_SIGNALS:
    //void executionStateChange_signal(const executionState_ec parserExecutionState_par_con);
    //this action errors
    //void addError_signal(const QString& error_par_con);
    //when the process "finishes" in any way: error, user stopped or successful
    //void anyFinish_signal();
};

#endif // STRINGPARSERMAPQTSO_PARSERBASE_HPP
