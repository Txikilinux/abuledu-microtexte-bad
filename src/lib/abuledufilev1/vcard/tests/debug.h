#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>
#include <QStringList>

//Source: http://brianmilco.blogspot.fr/2011/11/color-debug-output-with-qt-and-qdebug.html
inline QString debugFunctionName(QString name)
{
    QString output;
    QStringList classParts = name.split("::");
    QStringList nameAndReturnType = classParts.first().split(" ");
    QString returnType = ""; //ctor, dtor don't have return types
    if(nameAndReturnType.count() > 1)
        returnType = nameAndReturnType.first() + " ";
    QString className = nameAndReturnType.last();
    QStringList funcAndParamas = classParts.last().split("(");
    funcAndParamas.last().chop(1);
    QString functionName = funcAndParamas.first();
    QStringList params = funcAndParamas.last().split(",");
    output.append("\033[036m");
    output.append(returnType);
    output.append("\033[0m\033[32m");
    output.append(className);
    output.append("\033[0m::");
    output.append("\033[35m");
    output.append(functionName);
    output.append("\033[0m(");
    QStringList::const_iterator param;
    for (param = params.begin(); param != params.constEnd(); ++param) {
        if(param != params.begin()) {
            output.append("\033[0m,");
        }
        output.append("\033[036m");
        output.append((*param));
    }
    output.append("\033[0m)");
    return output;
}

#define DEBUG(message) \
( \
    (qDebug() << debugFunctionName(Q_FUNC_INFO).toStdString().c_str() << ":" << QString(message).toStdString().c_str()), \
    (void)0 \
)

#define WARN(message) \
( \
    (qWarning() << debugFunctionName(Q_FUNC_INFO).toStdString().c_str() << ":" << QString(message).toStdString().c_str()), \
    (void)0 \
)

#define CRITICAL(message) \
( \
    (qCritical() << debugFunctionName(Q_FUNC_INFO).toStdString().c_str() << ":" << QString(message).toStdString().c_str()), \
    (void)0 \
)

#define FATAL(message) \
( \
    (qFatal("%s : %s", debugFunctionName(Q_FUNC_INFO).toStdString().c_str(), QString(message).toStdString().c_str())), \
    (void)0 \
)

#endif // DEBUG_H
