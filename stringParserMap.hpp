#ifndef STRINGPARSERMAPQTSO_STRINGPARSERMAP_HPP
#define STRINGPARSERMAPQTSO_STRINGPARSERMAP_HPP

#include "crossPlatformMacros.hpp"
#include "parsers/base.hpp"

#include "baseClassQtso/baseClassQt.hpp"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QHash>
#include <QJsonObject>

#include <vector>

class EXPIMP_STRINGPARSERMAPQTSO stringParserMap_c : public QObject, public baseClassQt_c
{
    Q_OBJECT

    //key is order, value is parser
    QMap<int_fast64_t, parserBase_c*> orderToParserBaseMap_pri;
    //key is string trigger, value is order
    QHash<QString, int_fast64_t> stringTriggerToOrderMap_pri;
public:
    stringParserMap_c() = default;
    //-1 when not found
    int_fast64_t stringTriggerIndex_f(const QString& stringTrigger_par_con) const;
    //index >= size will be added at the end, negative index = error
    //using an existing index will shift the parser
    //object at that index and all that come after +1 position
    //first index is 0
    void addParser_f(parserBase_c* parserBase_par, const int_fast64_t index_par_con = INT64_MAX, const bool parentTheObject_par_con = true);
    int removeParser_f(const int_fast64_t index_par_con, const bool destroyToo_par_con = true);
    //it will shift existing ones like addParser_f, then remove the old one
    //doesn't accept negative index/s
    //"newIndex" beyond "size-index" will be treated as "size-index"
    bool changeParserIndex_f(const int_fast64_t sourceIndex_par_con, const int_fast64_t newIndex_par_con);

    void executeForString_f(QString* string_par);

    QMap<int_fast64_t, parserBase_c*> orderToParserBaseMap_f() const;

    void write_f(QJsonObject &json_ref_par) const;
    //existing parsers are kept and "read" are appended,
    //if there is a string trigger conflict the new read replaces the old
    void read_f(const QJsonObject &json_par_con);

    void clear_f();

    //returns a vectors of the stringTriggers already in use
    QList<QString> stringTriggers_f() const;

    static QString parseString_f(const QString& string_par_con, stringParserMap_c* stringParserMap_par);
    static QStringList parseStringList_f(const QStringList& stringList_par_con, stringParserMap_c* stringParserMap_par);
};

#endif // STRINGPARSERMAPQTSO_STRINGPARSERMAP_HPP
