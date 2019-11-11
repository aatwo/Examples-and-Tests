#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <type_traits>

int main( int argc, char** argv )
{
    std::filesystem::path filepathA("test-file-a.txt");
    std::filesystem::path filepathB("test-file-b.txt");

    { // Setup

        // Create a test file (note the fstream support for std::path)
        std::ofstream f(filepathA);
        f << "Hello, World!";
    }

    // Path operations

    { // Examining a relative path object

        std::filesystem::path path("fake_file.txt");

        std::cout << "Relative path " << path << std::endl;
        std::cout << "    root_name(): " << path.root_name() << std::endl;
        std::cout << "    root_path(): " << path.root_path() << std::endl;
        std::cout << "    relative_path(): " << path.relative_path() << std::endl;
        std::cout << "    parent_path(): " << path.parent_path() << std::endl;
        std::cout << "    filename(): " << path.filename() << std::endl;
        std::cout << "    stem(): " << path.stem() << std::endl;
        std::cout << "    extension(): " << path.extension() << std::endl;
        std::cout << std::endl;
    }

    { // Examining an absolute path object

        std::filesystem::path path("C:/fake_folder/fake_file.txt");

        std::cout << "Absolute path " << path << std::endl;
        std::cout << "    root_name(): " << path.root_name() << std::endl;
        std::cout << "    root_path(): " << path.root_path() << std::endl;
        std::cout << "    relative_path(): " << path.relative_path() << std::endl;
        std::cout << "    parent_path(): " << path.parent_path() << std::endl;
        std::cout << "    filename(): " << path.filename() << std::endl;
        std::cout << "    stem(): " << path.stem() << std::endl;
        std::cout << "    extension(): " << path.extension() << std::endl;

        std::cout << "    path components:" << std::endl;
        for(const std::filesystem::path& path_component : path)
            std::cout << "        " << path_component << std::endl;

        std::cout << std::endl;
    }

    { // Displaying the contents of a directory

        std::filesystem::path parent_dir = ".";
        std::cout << "Printing the contents of " << parent_dir << std::endl;
        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(parent_dir))
        {
            std::cout << "    " << entry.path() << std::endl;
        }
        std::cout << std::endl;
    }

    { // Displaying the contents of a directory

        std::filesystem::path parent_dir = "..";
        std::cout << "Printing the contents of " << parent_dir << std::endl;
        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(parent_dir))
        {
            std::cout << "    " << entry.path() << std::endl;
        }
        std::cout << std::endl;
    }

    { // Recursively displaying the contents of a directory and its sub directories

        std::filesystem::path parent_dir = ".";
        std::cout << "Recursively printing the contents of " << parent_dir << std::endl;
        for(const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(parent_dir))
        {
            std::cout << "    " << entry.path() << std::endl;
        }
        std::cout << std::endl;
    }

    { // Checking if a file exists
        
        bool filepath_a_exists = std::filesystem::exists(filepathA);
        bool filepath_b_exists = std::filesystem::exists(filepathB);

        std::cout << "Filepath A exists: " << (filepath_a_exists ? "true" : "false") << std::endl;
        std::cout << "Filepath B exists: " << (filepath_b_exists ? "true" : "false") << std::endl;
        std::cout << std::endl;
    }

    { // Filesystem errors

        try
        {
            std::filesystem::path path("as0-123//¿");
            std::cout << "Attempting to iterate file path as if it were a directory" << std::endl;
            for(const auto& entry : std::filesystem::directory_iterator(filepathA))
                std::cout << "    " << entry.path() << std::endl;
        }
        catch(const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        try
        {
            std::filesystem::path path("this-folder-doesnt-exist");
            std::cout << "Attempting to iterate directory that doesn't exist" << std::endl;
            for(const auto& entry : std::filesystem::directory_iterator(path))
                std::cout << "    " << entry.path() << std::endl;
        }
        catch(const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    std::cout << "\n\n";
    return 0;
}