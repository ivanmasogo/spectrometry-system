#ifndef PREDICTIONS_H
#define PREDICTIONS_H

#include <QString>
#include <QJsonObject>
#include <QProcess>
#include <QJsonDocument>

class Prediction {
public:
    Prediction();
    ~Prediction();

    QJsonObject makePredictions(const QString &csvFilePath);

};

#endif // PREDICTIONS_H
