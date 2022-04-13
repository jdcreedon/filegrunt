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
    char *sql;

    ui->setupUi(this);

    createActions();

    ui->MWplainTextEdit->appendPlainText("1. Select the source directory. ");
    ui->MWplainTextEdit->appendPlainText("2. Select the destination directory where duplicates will be copied to. ");
    ui->MWplainTextEdit->appendPlainText("3. Press Find Duplicates to find images that have the same name and size.");
    ui->MWplainTextEdit->appendPlainText("4. Press Compare images to compare the images from step 3 with each other to find duplicate images.");
    ui->MWplainTextEdit->appendPlainText("5. Press View Duplicates to view the original image file and the associated duplicate image files. ");
    ui->MWplainTextEdit->appendPlainText("6. Press Move Duplicates to copy the Duplicates to the destination directory.");
    ui->MWplainTextEdit->appendPlainText("Enjoy!");
    ui->MWplainTextEdit->appendPlainText("");

    ui->MWplainTextEdit->appendPlainText("Watch for status updates/errors here :-) :");

    // Save the connection result

    //auto rc = sqlite3_open("../data/offline.db", &db);  // File system database
    auto rc = sqlite3_open(":memory:", &db); // In Memory Database

    qDebug() << "DB Autocommit status :" << sqlite3_get_autocommit(db);

    std::filesystem::path p = "../data/offline.db";

    qDebug() << "absolute path" << std::filesystem::absolute(p).c_str();
    qDebug() << "current path" << std::filesystem::current_path().c_str();

    // Test if there was an error
    if (rc) {

        qDebug() << "DB Open Error: " << sqlite3_errmsg(db);

    } else {

        qDebug() << "Opened Database Successfully!";
    }

    // Source Index Table

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

    // Destination Index Table

    sql = "CREATE TABLE destination_index(directory text);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Table created successfully";
    }

    sql = "DELETE FROM destination_index;";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        qDebug() << "Table contents deleted successfully";
    }

    // File Index Table

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
        qDebug() << azColName[i] << argv[i]  << argv[i];
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
    connect(ui->actionSet_Destination, &QAction::triggered, this, &MainWindow::setDestination);
    connect(ui->actionMove_Duplicates, &QAction::triggered, this, &MainWindow::moveDuplicateImages);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::selectSource() {
    //printf("set source test");
    //setHidden(ui);
    char *zErrMsg = 0;
    //char *sql = "";

    QString directory = QFileDialog::getExistingDirectory(this,"Choose Folder","/home",QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog|QFileDialog::DontResolveSymlinks|QFileDialog::ReadOnly);

    if (directory.isEmpty())
        return;

    ui->MWplainTextEdit->appendPlainText("Input Directory: " + directory);

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

    //qDebug() << "Start of callback " << argv[0] ;

    int i=0;
    char *zErrMsg = 0;
    sqlite3* db1 = nullptr;
    std::string sql,sqlValues = "";
    db1 = (sqlite3 *) (const char *) NotUsed;
    qDebug() << "Start Recursive Directory Search: " << argv[i];

    try {
        for (auto const &file: std::filesystem::recursive_directory_iterator(argv[i])) {
            qDebug() << file.path().c_str();
            if (!file.is_directory()) {

                sql = "INSERT INTO file_index (path,name,size,type) VALUES ";
                sqlValues = sqlValues + "('" + file.path().string() + "','" + file.path().filename().string() + "','" +
                            std::to_string(file.file_size()) + "','" + file.path().extension().string() + "')";

                sql = sql + sqlValues;

                sqlValues = "";

                //// qDebug() << "Insert into file_index SQL is : " << sql.c_str();

                auto rc = sqlite3_exec(db1, sql.c_str(), callback, 0, &zErrMsg);

                sql = "";

                if (rc != SQLITE_OK) {
                    // qDebug() << zErrMsg;
                    sqlite3_free(zErrMsg);
                } else {
                    //// qDebug() << "File insertion completed successfully" << rc;
                }
            }
        }
    }
    catch(std::filesystem::filesystem_error &fse)
    {
        qDebug() << "Caught fatal exception: " << fse.what();
    }

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
        // qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        //// qDebug() << "Duplicate insertion completed successfully" << rc;
    }

    return 0;
}


void MainWindow::processSource() {

    char *zErrMsg = 0;
    ///char *sql = "";
    std::string image_types,sql;

    //1. Get files in source directories and add them to the file index table
    sql = "SELECT * from source_index";

    qDebug() << "Processing source";
    ui->MWplainTextEdit->appendPlainText("Processing Source...");

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

    ui->MWplainTextEdit->appendPlainText("Process Source Completed");

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

    ///// qDebug() << " = " << argv[i];

    return 0;
}

void MainWindow::compareImages(){

    char *zErrMsg = 0;
    //char *sql = "";
    std::string image_types, sql;

    // qDebug() << "Image Comparison Started";

    ui->MWplainTextEdit->appendPlainText("Image Comparison Started");

    //3.  Check if the duplicate image files contain the same data as well as having the same name and size
    // add video comparison exclude other file types

    // put the duplicates into a FIFO queue

    image_types = "SELECT extension from image_file_types";

    // sql = "SELECT path, name, size from file_index WHERE duplicate = 1 AND type IN (" + image_types + ") GROUP BY name, size ORDER BY name";

    sql = "SELECT path, name, size from file_index WHERE duplicate = 1 AND type IN (" + image_types + ") ORDER BY name";

    auto rc_duplicates = sqlite3_exec(db, sql.c_str(), duplicateToArrayCallback, MainWindow::dup_list, &zErrMsg);

    if( rc_duplicates != SQLITE_OK ){
        // qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        // qDebug() << "Duplicates identified successfully" << rc_duplicates;
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
            // qDebug() << i->path.c_str() << "Could not open or find the image";
        }

        // sudo apt install libgtk2.0-dev and pkg-config

        //cv::imshow("MainWindow", image1); //removed as causing error need to debug/fix

        for (i=duplicate_list.begin();i!=duplicate_list.end();i++){

            // compare the image to the subsequent entries that have the same file name
            if ((i->name == first_item.name) && (i->size == first_item.size)){
                // Read the image file
                cv::Mat image2 = cv::imread(i->path, cv::IMREAD_GRAYSCALE);

                // Check for failure
                if (image2.empty())
                {
                    // qDebug() << i->path.c_str() << "Could not open or find the image";
                }

                //compare the images, if the same then add the first_item to duplicate_images along with the path of the duplicate and end continue the for

                cv::Mat res_xor;
                bitwise_xor(image1, image2,res_xor);  // Result all zero means a match
                int matrix_zeros = countNonZero(res_xor);
                // qDebug() << "Matrix size: " << matrix_zeros;

                if (matrix_zeros == 0) {
                    // qDebug() << i->path.c_str() << "images match";

                    sql = "INSERT INTO duplicate_file_list (path, duplicate_path) VALUES ('"+first_item.path+"', '"+i->path+"') ";

                    auto rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

                    // put the duplicate image info into an array for use later when moving the image files

                    duplicate_data temp_struct;

                    temp_struct.duplicate_path= i->path;

                    duplicate_images.push_back(temp_struct);

                    sql ="";

                    if( rc != SQLITE_OK ){
                        // qDebug() << zErrMsg;
                        sqlite3_free(zErrMsg);
                    } else {
                        //// qDebug() << "Duplicate insertion completed successfully" << rc;
                    }

                } else {
                    // qDebug() << i->path.c_str() << "no image match";
                }

            }else{
                // not comparing same file name so stop the for loop and move onto the next item in the array to compare
                break;
            }

        }

    } while (duplicate_list.size() > 0);

    // qDebug() << "Image Comparison Complete";
    ui->MWplainTextEdit->appendPlainText("Image Comparison Completed");

}


void MainWindow::viewDuplicateImages(){

    qDebug() << "View Duplicates";
    ui->MWplainTextEdit->appendPlainText("View Duplicates - yet to be implemented");

}

void MainWindow::setDestination(){

    // qDebug() << "Set Destination";

    char *zErrMsg = 0;
    std::string sql;

    QString directory = QFileDialog::getExistingDirectory(this,"Choose Folder","/home",QFileDialog::ShowDirsOnly|QFileDialog::DontUseNativeDialog|QFileDialog::DontResolveSymlinks|QFileDialog::ReadOnly);

    MainWindow::destination_directory = directory.toStdString(); // Set the destination directory based on what the user has chosen

    if (directory.isEmpty())
        return;

    ui->MWplainTextEdit->appendPlainText("Destination directory: " + directory);

    sql = "INSERT INTO destination_index (directory) VALUES ('"+directory.toStdString()+"') ";

    auto rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        // qDebug() << zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        //// qDebug() << "insertion completed successfully" << rc;
    }

    //TODO: need to check permissions in destination folder

}

void MainWindow::moveDuplicateImages(){

    std::error_code ec;
    std::filesystem::path destination_sub_directory;
    // qDebug() << "Move Duplicates 1";

    // Iterate over the array and copy the files to the destination
    // maintain source directory structure within destination directory
    // this ensures that integrity of duplicates are maintained as there may be more than one duplicate copied
    // we don't want to overwrite files if we use a common/shared destination directory

    list<duplicate_data>::iterator i;
    std::string dup_count = std::to_string(duplicate_images.size());

    ui->MWplainTextEdit->appendPlainText("Moving " + QString::fromStdString(dup_count) + " duplicate image files");

    do {

        duplicate_data first_item = duplicate_images.front();
        duplicate_images.pop_front();

        std::filesystem::path source_path = first_item.duplicate_path;
        std::string destination_filename;

        destination_filename = source_path.filename().string();

        // create the directory structure for the duplicate file using the destination directory as the new root/parent
        // std::filesystem::create_directories("sandbox/1/2/a");

        if (source_path.root_name() == "C:"){  // remove the root from the destination directory also a windows directory
            //qDebug() << "root name not null" << source_path.root_name().c_str();
            ui->MWplainTextEdit->appendPlainText("root name not null " + QString::fromStdString(source_path.root_name().string()));

            std::string sub_dir = first_item.duplicate_path;

            sub_dir.erase(0, source_path.root_name().string().length());

            //source_path = '"' + first_item.duplicate_path + '"';
            source_path = first_item.duplicate_path;

            ui->MWplainTextEdit->appendPlainText("sub dir " + QString::fromStdString(sub_dir));

            //destination_sub_directory = '"' + MainWindow::destination_directory.string() + sub_dir;
            destination_sub_directory = MainWindow::destination_directory.string() + sub_dir;

            destination_sub_directory = destination_sub_directory.remove_filename().string();// + '"';

            ui->MWplainTextEdit->appendPlainText("destination sub directory " + QString::fromStdString(destination_sub_directory.string() ));

            std::filesystem::path windows_destination_sub_directory = MainWindow::destination_directory.string() + sub_dir;

            try {
                ui->MWplainTextEdit->appendPlainText("Creating Destination Sub Directory : " + QString::fromStdString(windows_destination_sub_directory.remove_filename().make_preferred().string()));
                std::filesystem::create_directories(windows_destination_sub_directory.remove_filename().make_preferred());
            }
            catch(std::filesystem::filesystem_error const& ex){
                ui->MWplainTextEdit->appendPlainText("Error message (Create Directory): " + QString::fromStdString(ex.code().message()));
            }

            if (std::filesystem::exists(destination_sub_directory.make_preferred().string())) {
                ui->MWplainTextEdit->appendPlainText("Destination Directory Exists: " + QString::fromStdString(windows_destination_sub_directory.make_preferred().string()));
            }else{
                ui->MWplainTextEdit->appendPlainText("Destination Directory Doesn't Exist: " + QString::fromStdString(windows_destination_sub_directory.make_preferred().string()));
            }
            windows_destination_sub_directory = windows_destination_sub_directory.string() + destination_filename;

            // Copy the file
            if (std::filesystem::exists(source_path.make_preferred())) {
                ui->MWplainTextEdit->appendPlainText("Source File Exists: " + QString::fromStdString(source_path.make_preferred().string()));
            }else{
                ui->MWplainTextEdit->appendPlainText("Source File Doesn't Exist: " + QString::fromStdString(source_path.make_preferred().string()));
            }
            ui->MWplainTextEdit->appendPlainText("Copying from: " + QString::fromStdString(source_path.make_preferred().string()) + " to: " + QString::fromStdString(windows_destination_sub_directory.make_preferred().string()));

            // copy the duplicate file to the destination directory
            try {
                std::filesystem::copy(source_path.make_preferred(), windows_destination_sub_directory.make_preferred());  // copy files only and not sub-directories
                //QFile::copy(QString::fromStdString(source_path.make_preferred().string()),QString::fromStdString(destination_sub_directory.make_preferred().string()+destination_filename));
            }
            catch(std::filesystem::filesystem_error const& ex) {
                std::cout
                        << "what():  " << ex.what() << '\n'
                        << "path1(): " << ex.path1() << '\n'
                        << "path2(): " << ex.path2() << '\n'
                        << "code().value():    " << ex.code().value() << '\n'
                        << "code().message():  " << ex.code().message() << '\n'
                        << "code().category(): " << ex.code().category().name() << '\n';
                std::string copy_error = "what():  " + (string) ex.what() + '\n' + "path1(): " + ex.path1().string() + '\n'
                                         + "path2(): " + ex.path2().string() + '\n'
                                         + "code().value():    " + char(ex.code().value()) + '\n'
                                         + "code().message():  " + ex.code().message().c_str() + '\n'
                                         + "code().category(): " + ex.code().category().name() + '\n';

                ui->MWplainTextEdit->appendPlainText("Error message: " +QString::fromStdString(copy_error));
            }

        }else{
            destination_sub_directory = MainWindow::destination_directory.string() + first_item.duplicate_path;
            destination_sub_directory = destination_sub_directory.remove_filename();

            try {
                ui->MWplainTextEdit->appendPlainText("Creating Destination Sub Directory : " + QString::fromStdString(destination_sub_directory.make_preferred().string()));
                std::filesystem::create_directories(destination_sub_directory.make_preferred());
            }
            catch(std::filesystem::filesystem_error const& ex){
                ui->MWplainTextEdit->appendPlainText("Error message (Create Directory): " + QString::fromStdString(ex.code().message()));
            }

            // Copy the file

            ui->MWplainTextEdit->appendPlainText("Copying from: " + QString::fromStdString(source_path.make_preferred().string()) + " to: " + QString::fromStdString(destination_sub_directory.make_preferred().string()+destination_filename));

            // copy the duplicate file to the destination directory
            try {
                std::filesystem::copy(source_path.make_preferred(), destination_sub_directory.make_preferred());  // copy files only and not sub-directories
                //QFile::copy(QString::fromStdString(source_path.make_preferred().string()),QString::fromStdString(destination_sub_directory.make_preferred().string()+destination_filename));
            }
            catch(std::filesystem::filesystem_error const& ex) {
                std::cout
                        << "what():  " << ex.what() << '\n'
                        << "path1(): " << ex.path1() << '\n'
                        << "path2(): " << ex.path2() << '\n'
                        << "code().value():    " << ex.code().value() << '\n'
                        << "code().message():  " << ex.code().message() << '\n'
                        << "code().category(): " << ex.code().category().name() << '\n';
                std::string copy_error = "what():  " + (string) ex.what() + '\n' + "path1(): " + ex.path1().string() + '\n'
                                         + "path2(): " + ex.path2().string() + '\n'
                                         + "code().value():    " + char(ex.code().value()) + '\n'
                                         + "code().message():  " + ex.code().message().c_str() + '\n'
                                         + "code().category(): " + ex.code().category().name() + '\n';

                ui->MWplainTextEdit->appendPlainText("Error message: " +QString::fromStdString(copy_error));
            }
        }


        if (std::filesystem::exists(source_path.make_preferred())) {
            ui->MWplainTextEdit->appendPlainText("Source File Exists: " + QString::fromStdString(source_path.make_preferred().string()));
        }else{
            ui->MWplainTextEdit->appendPlainText("Source File Doesn't Exist: " + QString::fromStdString(source_path.make_preferred().string()));
        }

        if (std::filesystem::exists(destination_sub_directory.make_preferred().string())) {
            ui->MWplainTextEdit->appendPlainText("Destination Directory Exists: " + QString::fromStdString(destination_sub_directory.make_preferred().string()));
        }else{
            ui->MWplainTextEdit->appendPlainText("Destination Directory Doesn't Exist: " + QString::fromStdString(destination_sub_directory.make_preferred().string()));
        }





        //qDebug() << "Error Code " << ex.message().c_str();

    } while (duplicate_images.size() > 0);

    ui->MWplainTextEdit->appendPlainText("Move completed");

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("FileGrunt <b>Application</b> find and move duplicate image files "));
}