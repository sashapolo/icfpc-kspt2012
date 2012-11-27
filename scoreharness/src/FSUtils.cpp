//#include <cstring>
//
//#include <boost/filesystem.hpp>
//
//#include "FSUtils.h"
//
//using namespace std;
//using namespace boost::filesystem;
//
//static vector<string> FSUtils::getDirFileList(const string & dirPath) {
//
//    if ( !exists( dirPath ) ) {
//        return false;
//    }
//    directory_iterator endItr; // default construction yields past-the-end
//    for ( directory_iterator itr( dirPath ); itr != endItr; ++itr ) {
//        if ( is_directory(itr->status()) ) {
//            if ( find_file( itr->path(), file_name, path_found ) ) {
//                return true;
//            }
//        } else if ( itr->leaf() == file_name ) {
//            path_found = itr->path();
//            return true;
//        }
//    }
//    return false;
//
//}
//
//FSUtils::FSUtils() {
//}
//
//FSUtils::FSUtils(const FSUtils& orig) {
//}
//
//FSUtils::~FSUtils() {
//}
