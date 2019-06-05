#include "stringParserMap.hpp"

#include "parsers/stringReplacer.hpp"

#ifdef DEBUGJOUVEN
#include "essentialQtso/essentialQt.hpp"
#endif

#include <QJsonArray>

int_fast64_t stringParserMap_c::stringTriggerIndex_f(const QString& stringTrigger_par_con) const
{
    int_fast64_t indexTmp(-1);
    QHash<QString, int_fast64_t>::const_iterator findResultTmp(stringTriggerToOrderMap_pri.find(stringTrigger_par_con));
    if (findResultTmp not_eq stringTriggerToOrderMap_pri.cend())
    {
        indexTmp = findResultTmp.value();
    }
    return indexTmp;
}

void stringParserMap_c::addParser_f(
        parserBase_c* parserBase_par
        , const int_fast64_t index_par_con
        , const bool parentTheObject_par_con)
{
    while (true)
    {
        if (parserBase_par == nullptr)
        {
            appendError_f("Trying to add a null parser pointer");
            break;
        }
        if (stringTriggerToOrderMap_pri.contains(parserBase_par->stringTrigger_f()))
        {
            appendError_f("Parser string trigger " + parserBase_par->stringTrigger_f() + " already in use");
            break;
        }
        //negative index/s
        if (index_par_con < 0)
        {
            appendError_f("Negative index");
            break;
        }

        int_fast64_t insertIndexTmp(index_par_con);
        if (index_par_con > -1 and index_par_con < orderToParserBaseMap_pri.size())
        {
            //shifting time, from the end to the iterator that's going to be replaced
            QMap<int_fast64_t, parserBase_c*>::const_iterator replacedIndexIteratorTmp(orderToParserBaseMap_pri.find(index_par_con));
            QMap<int_fast64_t, parserBase_c*>::const_iterator iteratorTmp(orderToParserBaseMap_pri.cend());
            //go the the end - 1, end has nothing/can't be accesed
            iteratorTmp--;
            //while the iterator is not the new index keep going backward and shifting the old elements
            while (iteratorTmp not_eq replacedIndexIteratorTmp)
            {
                stringTriggerToOrderMap_pri.insert(iteratorTmp.value()->stringTrigger_f(), iteratorTmp.key() + 1);
                orderToParserBaseMap_pri.insert(iteratorTmp.key() + 1, iteratorTmp.value());
                iteratorTmp--;
            }
            //"move" the object that is going to be replaced
            stringTriggerToOrderMap_pri.insert(iteratorTmp.value()->stringTrigger_f(), iteratorTmp.key() + 1);
            orderToParserBaseMap_pri.insert(iteratorTmp.key() + 1, iteratorTmp.value());
            //the only thing left is to replace the index where it was
        }
        else
        {
            insertIndexTmp = orderToParserBaseMap_pri.size();
        }

        orderToParserBaseMap_pri.insert(insertIndexTmp, parserBase_par);
        stringTriggerToOrderMap_pri.insert(parserBase_par->stringTrigger_f(), insertIndexTmp);

        if (parentTheObject_par_con)
        {
            parserBase_par->setParent(this);
        }
        break;
    }
}

int stringParserMap_c::removeParser_f(
        const int_fast64_t index_par_con
        , const bool destroyToo_par_con)
{
    int resultTmp(0);
    parserBase_c* objTmp(orderToParserBaseMap_pri.value(index_par_con, nullptr));
    if (objTmp not_eq nullptr)
    {
        resultTmp = orderToParserBaseMap_pri.remove(index_par_con);
        stringTriggerToOrderMap_pri.remove(objTmp->stringTrigger_f());
        if (destroyToo_par_con)
        {
            objTmp->deleteLater();
        }
    }
    return resultTmp;
}

bool stringParserMap_c::changeParserIndex_f(
        const int_fast64_t sourceIndex_par_con
        , const int_fast64_t newIndex_par_con)
{
    bool anyChangeResultTmp(false);
    QMap<int_fast64_t, parserBase_c*>::const_iterator oldIndexIteratorTmp(orderToParserBaseMap_pri.find(sourceIndex_par_con));
    parserBase_c* sourceValueTmp(nullptr);
    while (true)
    {
        //same index
        if (sourceIndex_par_con == newIndex_par_con)
        {
            break;
        }
        //negative index/s
        if (sourceIndex_par_con < 0 or newIndex_par_con < 0)
        {
            appendError_f("Negative index/s");
            break;
        }

        //source iterator doesn't exist
        if (oldIndexIteratorTmp == orderToParserBaseMap_pri.cend())
        {
            appendError_f("Source index not found");
            break;
        }
        sourceValueTmp = oldIndexIteratorTmp.value();

        //1 remove the source
        orderToParserBaseMap_pri.remove(sourceIndex_par_con);
        //2 all the items with a bigger index than the source must be shifted -1
        //shifting time, from source + 1 to the end, shift them -1
        for (int_fast64_t i = (sourceIndex_par_con + 1), l = orderToParserBaseMap_pri.size(); i < l; ++i)
        {
            stringTriggerToOrderMap_pri.insert(orderToParserBaseMap_pri.value(i)->stringTrigger_f(), i - 1);
            orderToParserBaseMap_pri.insert(i - 1, orderToParserBaseMap_pri.value(i));
        }

        //new index is at or beyond the end (insert at the end anyway)
        if (newIndex_par_con >= orderToParserBaseMap_pri.size())
        {
            stringTriggerToOrderMap_pri.insert(sourceValueTmp->stringTrigger_f(), orderToParserBaseMap_pri.size());
            orderToParserBaseMap_pri.insert(orderToParserBaseMap_pri.size(), oldIndexIteratorTmp.value());
            anyChangeResultTmp = true;
            break;
        }

        //new index is an existing one
        //shift everything +1 and replace index
        if (newIndex_par_con > -1 and newIndex_par_con < orderToParserBaseMap_pri.size())
        {
            //shifting time, from the end to the iterator that's going to be replaced
            QMap<int_fast64_t, parserBase_c*>::const_iterator replacedIndexIteratorTmp(orderToParserBaseMap_pri.find(newIndex_par_con));
            QMap<int_fast64_t, parserBase_c*>::const_iterator iteratorTmp(orderToParserBaseMap_pri.cend());
            //go the the end - 1, end has nothing/can't be accesed
            iteratorTmp--;
            //while the iterator is not the new index keep going backward and shifting the old elements
            while (iteratorTmp not_eq replacedIndexIteratorTmp)
            {
                stringTriggerToOrderMap_pri.insert(iteratorTmp.value()->stringTrigger_f(), iteratorTmp.key() + 1);
                orderToParserBaseMap_pri.insert(iteratorTmp.key() + 1, iteratorTmp.value());
                iteratorTmp--;
            }
            //one last time, "move" the item that's going to be replaced
            stringTriggerToOrderMap_pri.insert(iteratorTmp.value()->stringTrigger_f(), iteratorTmp.key() + 1);
            orderToParserBaseMap_pri.insert(iteratorTmp.key() + 1, iteratorTmp.value());
            //replace the index
            orderToParserBaseMap_pri.insert(newIndex_par_con, sourceValueTmp);
            stringTriggerToOrderMap_pri.insert(sourceValueTmp->stringTrigger_f(), newIndex_par_con);
            anyChangeResultTmp = true;
        }
        break;
    }

    return anyChangeResultTmp;
}

void stringParserMap_c::executeForString_f(QString* string_par)
{
    QMap<int_fast64_t, parserBase_c*>::const_iterator iteratorTmp(orderToParserBaseMap_pri.constBegin());
    while (iteratorTmp not_eq orderToParserBaseMap_pri.constEnd())
    {
        iteratorTmp.value()->execute_f(string_par);
        ++iteratorTmp;
    }
}

QMap<int_fast64_t, parserBase_c*> stringParserMap_c::orderToParserBaseMap_f() const
{
    return orderToParserBaseMap_pri;
}

void stringParserMap_c::write_f(QJsonObject& json_ref_par) const
{
    QMap<int_fast64_t, parserBase_c*>::const_iterator iteratorTmp(orderToParserBaseMap_pri.constBegin());
    QJsonArray parserArrayTmp;
    while (iteratorTmp not_eq orderToParserBaseMap_pri.constEnd())
    {
        QJsonObject jsonObjectTmp;
        iteratorTmp.value()->write_f(jsonObjectTmp);
        parserArrayTmp.append(jsonObjectTmp);
        ++iteratorTmp;
    }
    json_ref_par["parsers"] = parserArrayTmp;
}

void stringParserMap_c::read_f(const QJsonObject& json_par_con)
{
    QJsonArray arrayTmp(json_par_con["parsers"].toArray());
    for (const auto& item_ite_con : arrayTmp)
    {
        QJsonObject actionDataJsonObject(item_ite_con.toObject());
        parserBase_c* parserTmp(nullptr);
        bool rightTypeFoundTmp(false);
        while (true)
        {
            if (parserBase_c::strToTypeMap_sta_con.value(actionDataJsonObject["type"].toString().toLower()) == parserBase_c::type_ec::stringReplace)
            {
                stringReplacer_c* stringReplacerTmp(new stringReplacer_c);
                parserTmp = stringReplacerTmp;
                rightTypeFoundTmp = true;
                break;
            }
            break;
        }
        if (rightTypeFoundTmp)
        {
            parserTmp->setParent(this);
            parserTmp->read_f(actionDataJsonObject);
            stringTriggerToOrderMap_pri.insert(parserTmp->stringTrigger_f(), orderToParserBaseMap_pri.size());
            orderToParserBaseMap_pri.insert(orderToParserBaseMap_pri.size(), parserTmp);
#ifdef DEBUGJOUVEN
            //qDebug() << "" << endl;
#endif
        }
    }
}

void stringParserMap_c::clear_f()
{
    QMap<int_fast64_t, parserBase_c*>::const_iterator iteratorTmp(orderToParserBaseMap_pri.constBegin());
    while (iteratorTmp not_eq orderToParserBaseMap_pri.constEnd())
    {
        iteratorTmp.value()->deleteLater();
        ++iteratorTmp;
    }
    orderToParserBaseMap_pri.clear();
    stringTriggerToOrderMap_pri.clear();
}

stringParserMap_c* stringParserMap_ptr_ext = nullptr;
