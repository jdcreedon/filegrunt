//
// Created by damian on 19/01/2022.
//

#include "MainWindow.h"

namespace Ui{};

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    //std::string *sql;
    char *zErrMsg = 0;
    char *sql = "";

    ui->setupUi(this);

    createActions();

    // Save the connection result

    auto rc = sqlite3_open("../data/offline.db", &db);
    //auto rc = sqlite3_open(":memory:", &db);

    qDebug() << "DB Autocommit status :" << sqlite3_get_autocommit(db);

    // Test if there was an error
    if (rc) {

        qDebug() << "DB Open Error: " << sqlite3_errmsg(db);

    } else {

        qDebug() << "Opened Database Successfully!";
    }

    //const char * sql= "CREATE TABLE file_index(directory text, name text, size real, type text";
    sql = "CREATE TABLE source_index(directory text);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Table created successfully";
    }

    sql = "DELETE FROM source_index;";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Table contents deleted successfully";
    }

    sql = "CREATE TABLE file_index(path text, name text, size real, type text, duplicate integer);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Table created successfully";
    }

    sql = "DELETE FROM file_index;";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Table contents deleted successfully";
    }

    sql = "CREATE TABLE duplicate_file_list(path text, duplicate_path text);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Table created successfully";
    }

    sql = "DELETE FROM duplicate_file_list;";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Table contents deleted successfully";
    }

    sql = "CREATE TABLE image_file_types(type text, extension text)";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Image File Type Table created successfully";
    }

    sql = "CREATE UNIQUE INDEX uindex ON image_file_types (extension);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Index created created successfully";
    }

    sql = "INSERT INTO image_file_types (extension) VALUES ('.bmp'),('.dib'),('.jpeg'),('.jpg'),('.jp2'),('.png'),('.pbm'),('.pgm'),('.ppm'),('.sr'),('.ras'),('.tiff'),('.tif'),('.heic'),('.BMP'),('.DIB'),('.JPEG'),('.JPG'),('.JP2'),('.PNG'),('.PBM'),('.PGM'),('.PPM'),('.SR'),('.RAS'),('.TIFF'),('.TIF'),('.HEIC');";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Image File Type Table created successfully";
    }

}

int MainWindow::callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i = 0; i<argc; i++) {
        //qDebug() << azColName[i] << argv[i]  << argv[i];
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    return 0;
}

MainWindow::MainWindow::~MainWindow()
{
    delete ui;
    // Close the connection
    sqlite3_close(db);
}

void MainWindow::closeEvent( QCloseEvent* event ) {

    printf("exiting");
}

void MainWindow::createActions()
{
    connect(ui->actionSelect_Source, &QAction::triggered, this, &MainWindow::selectSource);
    connect(ui->actionFind_Duplicates, &QAction::triggered, this, &MainWindow::processSource);
    connect(ui->actionCompare_Images, &QAction::triggered, this, &MainWindow::compareImages);
    connect(ui->actionView_Duplicates, &QAction::triggered, this, &MainWindow::viewDuplicateImages);
}

void MainWindow::selectSource() {
    //printf("set source test");
    //setHidden(ui);
    char *zErrMsg = 0;
    //char *sql = "";

    QString directory = QFileDialog::getExistingDirectory(this,"Choose Folder","/home",QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog|QFileDialog::DontResolveSymlinks|QFileDialog::ReadOnly);

    if (directory.isEmpty())
        return;
    //ui->lineEdit->setText(directory);

    // Create model
    model = new QStringListModel(this);

    // Make data

    //List << directory;

    // Populate our model
    //model->setStringList(List);

    //ui->listView->setModel(model);

    // Add tp database table

    std::string sql = "INSERT INTO source_index (directory) VALUES(?)";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(
            db,            // the handle to your (opened and ready) database
            sql.c_str(),    // the sql statement, utf-8 encoded
            sql.length(),   // max length of sql statement
            &stmt,          // this is an "out" parameter, the compiled statement goes here
            nullptr);

    std::string newDirectory = directory.toStdString();

    sqlite3_bind_text(
            stmt,             // previously compiled prepared statement object
            1,                // parameter index, 1-based
            newDirectory.c_str(),  // the data
            directory.length(), // length of data
            SQLITE_STATIC);   // this parameter is a little tricky - it's a pointer to the callback
    // function that frees the data after the call to this function.
    // It can be null if the data doesn't need to be freed, or like in this case,
    // special value SQLITE_STATIC (the data is managed by the std::string
    // object and will be freed automatically).

    sqlite3_step(stmt);

}

int MainWindow::getFilesCallback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    char *zErrMsg = 0;
    sqlite3* db1 = nullptr;
    std::string sql,sqlValues = "";
    db1 = (sqlite3 *) (const char *) NotUsed;

    //for(i = 0; i<argc; i++) {
        for (const auto & file : recursive_directory_iterator(argv[i])) {
            qDebug() << file.path();
            if (!file.is_directory()){

                sql = "INSERT INTO file_index (path,name,size,type) VALUES ";
                sqlValues = sqlValues + "('" + file.path().string() + "','" + file.path().filename().string() + "','" + std::to_string(file.file_size()) + "','" + file.path().extension().string() + "')";

                sql = sql + sqlValues;

                sqlValues = "";

                //qDebug() << "Insert into file_index SQL is : " << sql.c_str();

                auto rc = sqlite3_exec(db1, sql.c_str(), callback, 0, &zErrMsg);

                sql ="";

                if( rc != SQLITE_OK ){
                    qDebug() << zErrMsg;
                    sqlite3_free(zErrMsg);
                } else {
                    //qDebug() << "File insertion completed successfully" << rc;
                }

            }

        }
        //qDebug() << azColName[i] << argv[i]  << argv[i];
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    //}

    return 0;
}

int MainWindow::getDuplicateFileNamesCallback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    char *zErrMsg = 0;
    sqlite3* db1 = nullptr;
    std::string sql,sqlValues = "";
    db1 = (sqlite3 *) (const char *) NotUsed;

    sql = "UPDATE file_index set duplicate = 1 WHERE (name = '" + std::string(argv[0]) + "' AND size = " + argv[1] + ");";

    auto rc = sqlite3_exec(db1, sql.c_str(), callback, 0, &zErrMsg);

    sql ="";

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        //qDebug() << "Duplicate insertion completed successfully" << rc;
    }

    return 0;
}


void MainWindow::processSource() {

    char *zErrMsg = 0;
    ///char *sql = "";
    std::string image_types,sql;

    //1. Get files in source directories and add them to the file index table
    sql = "SELECT * from source_index";

    auto rc = sqlite3_exec(db, sql.c_str(), getFilesCallback, MainWindow::db, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "File List returned successfully" << rc;
    }

    //2. Remove the files that are not in the file extension types we are comparing

    image_types = "SELECT extension from image_file_types";

    // sql = "SELECT path, name, size from file_index WHERE duplicate = 1 AND type IN (" + image_types + ") GROUP BY name, size ORDER BY name";

    sql = "DELETE from file_index WHERE type NOT IN (" + image_types + ")";

    auto rc_duplicates = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if( rc_duplicates != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Deleted unneeded files" << rc_duplicates;
    }

    //3. Check for duplicate names and sizes in the name column and update file index table and mark as potential duplicates

    sql = "SELECT name, size, COUNT(*) from file_index GROUP BY name, size HAVING COUNT(*)>1";

    auto rc_duplicate_filename = sqlite3_exec(db, sql.c_str(), getDuplicateFileNamesCallback, MainWindow::db, &zErrMsg);

    if( rc_duplicate_filename != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Duplicates identified successfully" << rc_duplicate_filename;
    }

}

int MainWindow::duplicateToArrayCallback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    std::list<duplicate_data>* head;

    head = (std::list<duplicate_data> *) NotUsed;

    duplicate_data temp_struct;

    temp_struct.path = argv[0];
    temp_struct.name = argv[1];
    temp_struct.size = argv[2];

    head->push_back(temp_struct);

    ///qDebug() << " = " << argv[i];

    return 0;
}

void MainWindow::compareImages(){

    char *zErrMsg = 0;
    //char *sql = "";
    std::string image_types, sql;

    qDebug() << "Image Comparison Started";

    //3.  Check if the duplicate image files contain the same data as well as having the same name and size
    // add video comparison exclude other file types

    // put the duplicates into a FIFO queue

    image_types = "SELECT extension from image_file_types";

    // sql = "SELECT path, name, size from file_index WHERE duplicate = 1 AND type IN (" + image_types + ") GROUP BY name, size ORDER BY name";

    sql = "SELECT path, name, size from file_index WHERE duplicate = 1 AND type IN (" + image_types + ") ORDER BY name";

    auto rc_duplicates = sqlite3_exec(db, sql.c_str(), duplicateToArrayCallback, MainWindow::dup_list, &zErrMsg);

    if( rc_duplicates != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Duplicates identified successfully" << rc_duplicates;
    }

    // step through the queue
    // compare the nth name with n+1 name and if the same pop the nth name into another temp FIFO queue
    // if not then pop the n+1 onto the temp array/FIFO queue? and begin the image comparison loop for the tmp queue
    // loop through tmp FIFO queue and for each entry compare the nth with n+1, n+2, length of queue -1
    // clear out the temp FIFO queue and continue on looping through outer/main FIFO queue holding all duplicates

    list<duplicate_data>::iterator i;

    //pop the first item in the list and compare it against the subsequent entries that have the same file name

    do {
        duplicate_data first_item = duplicate_list.front();
        duplicate_list.pop_front();

        // Read the image file
        cv::Mat image1 = cv::imread(first_item.path, cv::IMREAD_GRAYSCALE);

        // Check for failure
        if (image1.empty())
        {
            qDebug() << i->path.c_str() << "Could not open or find the image";
        }

        for (i=duplicate_list.begin();i!=duplicate_list.end();i++){

            // compare the image to the subsequent entries that have the same file name
            if ((i->name == first_item.name) && (i->size == first_item.size)){
                // Read the image file
                cv::Mat image2 = cv::imread(i->path, cv::IMREAD_GRAYSCALE);

                // Check for failure
                if (image2.empty())
                {
                    qDebug() << i->path.c_str() << "Could not open or find the image";
                }

                //compare the images, if the same then add the first_item to duplicate_images along with the path of the duplicate and end continue the for

                cv::Mat res_xor;
                bitwise_xor(image1, image2,res_xor);  // Result all zero means a match
                int matrix_zeros = countNonZero(res_xor);
                qDebug() << "Matrix size: " << matrix_zeros;

                if (matrix_zeros == 0) {
                    qDebug() << i->path.c_str() << "images match";

                    sql = "INSERT INTO duplicate_file_list (path, duplicate_path) VALUES ('"+first_item.path+"', '"+i->path+"') ";

                    auto rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

                    sql ="";

                    if( rc != SQLITE_OK ){
                        qDebug() << zErrMsg;
                        sqlite3_free(zErrMsg);
                    } else {
                        //qDebug() << "Duplicate insertion completed successfully" << rc;
                    }

                } else {
                    qDebug() << i->path.c_str() << "no image match";
                }

            }else{
                // not comparing same file name so stop the for loop and move onto the next item in the array to compare
                break;
            }

        }

    } while (duplicate_list.size() > 0);

    qDebug() << "Image Comparison Complete";

}


void MainWindow::viewDuplicateImages(){

    qDebug() << "View Duplicates";

}