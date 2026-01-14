#pragma once



#include <QString>

#include <QVector>

#include <QStringList>

#include "overseer-types.h"



class DataIngestor {

public:

    bool loadFile(const QString& filePath);

    const QVector<IngestedDataRow>& getData() const { return data_; }

    const QStringList& getColumns() const { return columns_; }

    IngestedDataRow* findMatch(const QString& fieldName, const QString& searchText);

    QStringList getFieldValues(const QString& fieldName) const;

    void clear();

    bool hasData() const { return !data_.isEmpty(); }



private:

    QVector<IngestedDataRow> data_;

    QStringList columns_;

    bool parseCSV(const QString& content);

    bool parsePlainText(const QString& content);

    QString detectFormat(const QString& filePath);

};
