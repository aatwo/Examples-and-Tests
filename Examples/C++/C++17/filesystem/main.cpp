#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <type_traits>

int main( int argc, char** argv )
{
    std::filesystem::path filepathA("test-file-a.txt");
    std::filesystem::path filepathB("test-file-b.txt");

    { // Example file, folder and recursive folder removal (also cleanup after last runs created files + folders)
        std::filesystem::remove("test-file-a.txt");
        std::filesystem::remove_all("created-folder");
        std::filesystem::remove_all("copied-folder-deep");
        std::filesystem::remove("copied-folder-shallow");
    }

    { // Setup

        // Create a test file (note the fstream support for std::path)
        std::ofstream f(filepathA);
        f << "Hello, World!";
    }
    

    { // Evaluating a file

        std::cout << "File size of " << filepathA << " is " << std::filesystem::file_size(filepathA) << " bytes...\n";
    }

    // Path operations

    { // current_path()
        std::cout << "Current path: " << std::filesystem::current_path() << "\n";
    }

    { // Examining a relative path object

        std::filesystem::path path("fake_file.txt");

        std::cout << "Relative path " << path << "\n";
        std::cout << "    root_name(): " << path.root_name() << "\n";
        std::cout << "    root_path(): " << path.root_path() << "\n";
        std::cout << "    relative_path(): " << path.relative_path() << "\n";
        std::cout << "    parent_path(): " << path.parent_path() << "\n";
        std::cout << "    filename(): " << path.filename() << "\n";
        std::cout << "    stem(): " << path.stem() << "\n";
        std::cout << "    extension(): " << path.extension() << "\n";
        std::cout << "    string(): " << path.string() << "\n";
        std::cout << "    generic_string(): " << path.generic_string() << "\n";

        // TODO: path.native()
    }

    { // Examining an absolute path object

        std::filesystem::path path("C:/fake_folder/fake_file.txt");

        std::cout << "Absolute path " << path << "\n";
        std::cout << "    root_name(): " << path.root_name() << "\n";
        std::cout << "    root_path(): " << path.root_path() << "\n";
        std::cout << "    relative_path(): " << path.relative_path() << "\n";
        std::cout << "    parent_path(): " << path.parent_path() << "\n";
        std::cout << "    filename(): " << path.filename() << "\n";
        std::cout << "    stem(): " << path.stem() << "\n";
        std::cout << "    extension(): " << path.extension() << "\n";
        std::cout << "    string(): " << path.string() << "\n";
        std::cout << "    generic_string(): " << path.generic_string() << "\n";
        std::cout << "    path components:" << "\n";
        for(const std::filesystem::path& path_component : path)
            std::cout << "        " << path_component << "\n";
    }

    { // Filepath operations

        // Operator: /=
        std::filesystem::path path_a("C:/fake_folder");
        path_a /= "fake_file1.txt";
        std::cout << "Path_a after /= operation: " << path_a << "\n";

        // Operator: += (note this operator requires the separator to be added manually)
        std::filesystem::path path_b("C:/fake_folder");
        path_b += "/fake_file2.txt";
        std::cout << "Path_b after += operation: " << path_b << "\n";
    }

    { // Displaying the contents of a directory

        std::filesystem::path parent_dir = ".";
        std::cout << "Printing the contents of " << parent_dir << "\n";
        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(parent_dir))
        {
            std::cout << "    " << entry.path() << "\n";
        }
    }

    { // Displaying the contents of another directory

        std::filesystem::path parent_dir = "..";
        std::cout << "Printing the contents of " << parent_dir << "\n";
        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(parent_dir))
        {
            std::cout << "    " << entry.path() << "\n";
        }
    }

    { // Recursively displaying the contents of a directory and its sub directories

        std::filesystem::path parent_dir = ".";
        std::cout << "Recursively printing the contents of " << parent_dir << "\n";
        for(const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(parent_dir))
        {
            std::cout << "    " << entry.path() << "\n";
        }
    }

    { // Filesystem errors

        try
        {
            std::cout << "Attempting to iterate file path as if it were a directory\n";
            for(const auto& entry : std::filesystem::directory_iterator(filepathA))
                std::cout << "    " << entry.path() << "\n";
        }
        catch(const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }

        try
        {
            std::filesystem::path path("this-folder-doesnt-exist");
            std::cout << "Attempting to iterate directory that doesn't exist\n";
            for(const auto& entry : std::filesystem::directory_iterator(path))
                std::cout << "    " << entry.path() << "\n";
        }
        catch(const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    { // Checking if a file exists
        
        bool filepath_a_exists = std::filesystem::exists(filepathA);
        bool filepath_b_exists = std::filesystem::exists(filepathB);

        std::cout << "Filepath A exists: " << (filepath_a_exists ? "true" : "false") << "\n";
        std::cout << "Filepath B exists: " << (filepath_b_exists ? "true" : "false") << "\n";
    }

    { // Create a directory

        std::filesystem::path path("created-folder");
        std::cout << "Creating folder: '" << path.string() << "'...\n";
        std::filesystem::create_directory(path);
        
        if(std::filesystem::exists(path))
            std::cout << "    Success. Folder exists.\n";
        else
            std::cout << "    Failed.\n";
    }

    { // Create a directory chain

        std::filesystem::path path("created-folder/sub-folder-a/sub-folder-b/sub-folder-c/sub-folder-d/sub-folder-d");
        std::cout << "Creating chain of folders: '" << path.string() << "'...\n";
        std::filesystem::create_directories(path);
        
        if(std::filesystem::exists(path))
            std::cout << "    Success. Folder exists.\n";
        else
            std::cout << "    Failed.\n";
    }

    { // Copying a file

        std::filesystem::path target_folder("created-folder/sub-folder-a/sub-folder-b/sub-folder-c/sub-folder-d/sub-folder-d");
        target_folder /= "test-file-a-copy.txt";

        std::cout << "Copying test-file-a.txt to test-file-a-copy.txt...\n";
        std::filesystem::copy(filepathA, target_folder);
        if(std::filesystem::exists(target_folder))
            std::cout << "    Success, file exists.\n";
        else
            std::cout << "    Failed.\n";
    }

    { // Copying a file without specifying a name

        try {
            std::filesystem::path target_folder("created-folder/sub-folder-a/sub-folder-b/sub-folder-c/sub-folder-d/sub-folder-d");
            target_folder /= filepathA.filename();

            std::cout << "Copying test-file-a.txt...\n";
            std::filesystem::copy(filepathA, target_folder);
            if(std::filesystem::exists(target_folder))
                std::cout << "    Success, file exists.\n";
            else
                std::cout << "    Failed.\n";
        }
        catch(const std::exception& e)
        {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    // Note: copy_file() achieves the same as copy() if a file is specified, however the intent is more specific and will not work for directories.

    { // Copying a directory (without copying any of the sub-contents)

        std::filesystem::path source_folder("created-folder");
        std::filesystem::path target_folder("copied-folder-shallow");
        std::cout << "Copying folder (shallow copy): " << source_folder << " to " << target_folder << "\n";
        std::filesystem::copy(source_folder, target_folder);
        if(std::filesystem::exists(target_folder))
            std::cout << "    Success, copied folder exists.\n";
        else
            std::cout << "    Failed.\n";
    }

    { // Copying a directory recursively with all sub-contents

        std::filesystem::path source_folder("created-folder");
        std::filesystem::path target_folder("copied-folder-deep");
        std::cout << "Copying folder recursively (deep copy): " << source_folder << " to " << target_folder << "\n";
        std::filesystem::copy(source_folder, target_folder, std::filesystem::copy_options::recursive);
        if(std::filesystem::exists(target_folder))
            std::cout << "    Success, copied folder exists.\n";
        else
            std::cout << "    Failed.\n";
    }

    std::cout << "\n\n";
    return 0;
}