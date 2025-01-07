#include "headers/predictions.h"
#include <QProcess>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QString>

Prediction::Prediction() {}
Prediction::~Prediction() {}

QJsonObject Prediction::makePredictions(const QString &csvFilePath) {

    // Ruta donde está el script predict.py
    QString basePath = QCoreApplication::applicationDirPath();
    QDir dirBase(basePath);
    dirBase.cdUp();
    QString predictorPath = dirBase.absoluteFilePath("Predictor");

    QDir dir(predictorPath);
    // Listar los archivos en el directorio para depuración
    if (dir.exists()) {
        QStringList files = dir.entryList(QDir::Files); // Listar solo los archivos

        qDebug() << "Archivos en el directorio:" << predictorPath;
        for (const QString &file : files) {
            qDebug() << file;
        }

    } else {
        qDebug() << "El directorio no existe:" << predictorPath;
    }



    QString program;
    if (QSysInfo::productType() == "windows") {
        program = "python";
    } else {
        program = "python3";
    }

    QStringList arguments;
    QProcess process;


    arguments << "predictor.py" << csvFilePath; // Pasar el archivo CSV como argumento
    process.setWorkingDirectory(predictorPath);

    // Iniciar el proceso
    process.start(program, arguments);
    if (!process.waitForFinished()) {
        qDebug() << "Error en la ejecución del proceso";
        return QJsonObject(); // Retorna un objeto vacío si hay error
    }

    // Capturar la salida del script Python
    QByteArray output = process.readAllStandardOutput();
    QByteArray errorOutput = process.readAllStandardError();

    if (!errorOutput.isEmpty()) {
        qDebug() << "Error en la ejecución del script:" << errorOutput;
        return QJsonObject(); // Retorna un objeto vacío si hay un error
    }
    QString outputStr = QString(output);
    // Mostrar la salida para depurar
    qDebug() << "Salida del script:" << outputStr;
    // Parsear la salida JSON del Python
    QJsonDocument jsonDoc = QJsonDocument::fromJson(outputStr.toUtf8());
    if (jsonDoc.isNull()) { qDebug() << "La salida no es un JSON válido.";
        return QJsonObject(); // Retorna un objeto vacío si la salida no es un JSON válido
    }
    QJsonObject jsonObject = jsonDoc.object();
    return jsonObject; // Devolver el objeto JSON con los resultados
}
