#include "tests.h"
#include <algorithm>
#include <fstream>
#include <cassert>
#include <string>
#include <iostream>
#include <filesystem>

Book create_sample_book(int id) {
    const std::vector<std::string> genres = { "SF", "romance", "action", "history", "fantasy" };
    std::string genre = genres[id % genres.size()];
    return Book("Title" + std::to_string(id), "Author" + std::to_string(id), genre, 2000 + id, id);
}

void populate_repo(Repo& repo) {
    for (int i = 1; i <= 5; i++) {
        repo.Add(create_sample_book(i));
    }
}

void test_domain() {
    // Test constructor and getters
    Book b1("Dune", "Frank Herbert", "SF", 1965, 1);
    assert(b1.get_title() == "Dune");
    assert(b1.get_author() == "Frank Herbert");
    assert(b1.get_genre() == "SF");
    assert(b1.get_year() == 1965);
    assert(b1.get_id() == 1);

    // Test equality operator
    Book b2("Dune", "Frank Herbert", "SF", 1965, 1);
    Book b3("The Hobbit", "J.R.R. Tolkien", "fantasy", 1937, 2);
    assert(b1 == b2);
    assert(!(b1 == b3));

    // Test copy constructor
    Book b4(b1);
    assert(b4 == b1);

    // Test assignment operator
    Book b5;
    b5 = b3;
    assert(b5 == b3);

    // Test print_book
    b1.print_book();

    std::cout << "Domain tests passed!\n";
}

void test_repo_interface() {
    // Test with InMemoryRepo (unchanged)
    {
        Repo* repo = new InMemoryRepo();
        test_repo_operations(repo);
        delete repo;
    }

    // Test with FileRepo
    {
        const std::string test_file = "test_repo.txt";
        std::remove(test_file.c_str());  // Cleanup

        // Phase 1: Create and populate file
        {
            Repo* repo = new FileRepo(test_file);
            // Add test data
            repo->Add(create_sample_book(1));
            repo->Add(create_sample_book(2));
            delete repo;  // This should trigger save
        }

        // Phase 2: Verify file content
        {
            std::ifstream file(test_file);
            assert(file.good());

            std::string line;
            int line_count = 0;
            while (std::getline(file, line)) {
                if (!line.empty()) line_count++;
            }
            file.close();

            std::cout << "Found " << line_count << " books in file" << std::endl;
            assert(line_count == 2);  // We added 2 books
        }

        // Phase 3: Verify loading works
        {
            Repo* repo = new FileRepo(test_file);
            assert(repo->get_list().size() == 2);
            delete repo;
        }

        std::remove(test_file.c_str());  // Cleanup
    }
}

void test_repo_operations(Repo* repo) {
    // Test empty repo
    assert(repo->get_list().empty());
    assert(repo->find_id(1) == -1);
    assert(repo->find_book_title("Test") == -1);

    // Test Add and get_list
    Book b1 = create_sample_book(1);
    assert(repo->Add(b1) == 1);
    assert(repo->get_list().size() == 1);
    assert(repo->get_list()[0] == b1);

    // Test duplicate add
    assert(repo->Add(b1) == 0);
    assert(repo->get_list().size() == 1);

    // Test find_id
    assert(repo->find_id(1) == 0);
    assert(repo->find_id(999) == -1);

    // Test find_book_title
    assert(repo->find_book_title("Title1") == 0);
    assert(repo->find_book_title("Nonexistent") == -1);

    // Test modify
    Book b2("New Title", "New Author", "romance", 2020, 1);
    assert(repo->modify(b2) == 1);
    assert(repo->get_list()[0].get_title() == "New Title");
    assert(repo->modify(create_sample_book(999)) == 0);

    // Test Delete_book
    assert(repo->Delete_book(1) == 1);
    assert(repo->get_list().empty());
    assert(repo->Delete_book(999) == 0);
}

void test_file_repo_specific() {
    const std::string test_file = "test_file_repo.txt";
    // Clean up in case previous test failed
    std::remove(test_file.c_str());

    // Test loading from non-existent file
    {
        FileRepo repo(test_file);
        assert(repo.get_list().empty()); // Should start empty
    }

    // Create test data
    {
        FileRepo repo(test_file);
        repo.Add(create_sample_book(1));
        repo.Add(create_sample_book(2));
    }

    // Test loading from existing file
    {
        FileRepo repo(test_file);
        assert(repo.get_list().size() == 2);
        assert(repo.find_id(1) != -1);
        assert(repo.find_id(2) != -1);
    }

    // Test that changes are persisted
    {
        FileRepo repo(test_file);
        repo.Delete_book(1);
        assert(repo.get_list().size() == 1);
    }
    {
        FileRepo repo(test_file);
        assert(repo.get_list().size() == 1); // Verify deletion was saved
    }

    // Clean up
    std::remove(test_file.c_str());
    std::cout << "FileRepo specific tests passed!\n";
}

void test_in_memory_repo_specific() {
    InMemoryRepo repo;

    // Verify it doesn't try to save/load from file
    repo.Add(create_sample_book(1));
    repo.Add(create_sample_book(2));
    assert(repo.get_list().size() == 2);

    // Shouldn't affect any files on disk
    assert(!std::ifstream("nonexistent.txt").good());

    std::cout << "InMemoryRepo specific tests passed!\n";
}

void test_service() {
    // Test with InMemoryRepo
    {
        Repo* repo = new InMemoryRepo();
        test_service_operations(repo);
        //delete repo;
    }

    // Test with FileRepo
    {
        const std::string test_file = "test_service.txt";
        std::remove(test_file.c_str());

        Repo* repo = new FileRepo(test_file);
        test_service_operations(repo);
        //delete repo;

        std::remove(test_file.c_str());
    }
}

void test_service_operations(Repo* repo) {
    Service* service = new Service(repo);

    service->clearUndoActions();


    // Test empty service
    assert(service->get_list_serv().empty());
    assert(service->find_serv(1) == -1);

    // Test Delete_serv on empty list - should throw EmptyListException
    bool emptyListExceptionThrown = false;
    try {
        service->Delete_serv(1);
    }
    catch (const EmptyListException&) {
        emptyListExceptionThrown = true;
    }
    assert(emptyListExceptionThrown);

    // Now add some books
    assert(service->Add_serv("Valid Title", "Valid Author", "action", 2020, 100) == 1);
    assert(service->get_list_serv().size() == 1);

    service->clearUndoActions();
    // Test Delete_serv with invalid ID - should throw BookNotFoundException
    bool bookNotFoundExceptionThrown = false;
    try {
        service->Delete_serv(999);
    }
    catch (const BookNotFoundException&) {
        bookNotFoundExceptionThrown = true;
    }
    assert(bookNotFoundExceptionThrown);

    // Test successful deletion
    assert(service->Delete_serv(100) == 1);
    assert(service->get_list_serv().empty());

    service->clearUndoActions();

    // Test Modify_serv
    service->Add_serv("To Modify", "Author", "history", 2010, 200);
    assert(service->Modify_serv("Modified", "Author", "fantasy", 2011, 200) == 1);

    try {
        service->Modify_serv("Title", "Author", "invalid", 2020, 200);
        assert(false);
    }
    catch (const ValidationException&) {}

    try {
        service->Modify_serv("Title", "Author", "SF", 2020, 999);
        assert(false);
    }
    catch (const BookNotFoundException&) {}

    service->Add_serv("Book1", "Author1", "SF", 2001, 101);
    service->Add_serv("Book2", "Author2", "fantasy", 2002, 102);
    service->Add_serv("Book3", "Author3", "romance", 2003, 103);
    service->Add_serv("Book4", "Author4", "action", 2004, 104);

    // Get the book list reference
    vector<Book>& book_list = service->get_list_serv();

    // Test filter
    auto filtered = service->filter(book_list, 2003);
    assert(filtered.size() == 3); // Should get books from 2003 and 2004

    // Test sort
    service->sort(book_list, 0, [](const Book& a, const Book& b) {
        return a.get_title() < b.get_title();
        });
    assert(std::is_sorted(book_list.begin(), book_list.end(),
        [](const Book& a, const Book& b) { return a.get_title() < b.get_title(); }));

    // Test descending sort
    service->sort(book_list, 1, [](const Book& a, const Book& b) {
        return a.get_title() < b.get_title();
        });
    assert(std::is_sorted(book_list.begin(), book_list.end(),
        [](const Book& a, const Book& b) { return a.get_title() > b.get_title(); }));

    Book validBook1("", "Real Author", "SF", 2020, 1);
    assert(service->validator(validBook1) == 0);

    Book validBook2("Book", "", "SF", 2020, 1);
    assert(service->validator(validBook2) == 0);

    Book validBook3("Book", "Real Author", "", 2020, 1);
    assert(service->validator(validBook3) == 0);

    Book validBook4("Book", "Real Author", "SF", 2900, 1);
    assert(service->validator(validBook4) == 0);

    bool exceptionThrown = false;
    try {
        service->Add_serv("", "Valid Author", "SF", 2020, 2);
    }
    catch (const ValidationException&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    delete service;
    std::cout << "Service tests passed!\n";
}

void test_basket() {
    Basket basket;
    Repo* repo = new InMemoryRepo();
    Service* service = new Service(repo);

    // Test empty basket
    assert(basket.get_basket().empty());
    bool exceptionThrown = false;
    try {
        service->delete_whole_basket();
    }
    catch (const  EmptyListException&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // Populate service with books
    service->Add_serv("Book1", "Author1", "SF", 2001, 1);
    service->Add_serv("Book2", "Author2", "fantasy", 2002, 2);
    auto& books = service->get_list_serv();

    // Test add_to_basket
    basket.add_to_basket(books[0]);
    assert(basket.get_basket().size() == 1);
    assert(basket.get_basket()[0] == books[0]);

    // Test delete_by_title
    std::string title = books[0].get_title();
    basket.delete_by_title(title);
    assert(basket.get_basket().empty());

    // Test empty_basket
    basket.add_to_basket(books[0]);
    basket.add_to_basket(books[1]);
    basket.empty_basket();
    assert(basket.get_basket().empty());

    // Test generate
    basket.generate(2, books);
    assert(basket.get_basket().size() == 2);

    // Test export_basket
    assert(basket.export_basket("test_export.csv") == 1);

    // Verify file was created
    std::ifstream file("test_export.csv");
    assert(file.good());
    file.close();
    std::remove("test_export.csv");

    delete service;
    std::cout << "Basket tests passed!\n";
}

void test_service_basket_operations() {
    Repo* repo = new InMemoryRepo();
    Service* service = new Service(repo);

    std::string book1 = "Book1";
    std::string book2 = "Book2";
    std::string book3 = "Book3";
    std::string invalid_book = "Nonexistent Book";
    std::string export_file = "test_export.csv";
    std::string invalid_file = "invalid.txt";

    bool emptyListExceptionThrown = false;
    try {
        service->delete_basket_serv(book1);
        assert(false && "Should have thrown EmptyListException");
    }
    catch (const EmptyListException& e) {
        emptyListExceptionThrown = true;
        assert(std::string(e.what()) == "The basket is already empty");
    }
    assert(emptyListExceptionThrown);

    assert(service->Add_serv(book1, "Author1", "SF", 2001, 1) == 1);
    assert(service->Add_serv(book2, "Author2", "fantasy", 2002, 2) == 1);

    service->add_to_basket_serv(book1);
    assert(service->get_basket_serv().size() == 1);

    bool fileExceptionThrown = false;
    try {
        service->export_basket_serv("NUL:\\cannot_write_here.csv");
    }
    catch (const FileException& e) {
        fileExceptionThrown = true;
        assert(std::string(e.what()) == "File could not be opened");
    }
    assert(fileExceptionThrown);

    try {
        service->add_to_basket_serv(invalid_book);
        assert(false && "Should have thrown BookNotFoundException");
    }
    catch (const BookNotFoundException&) {
    }

    service->delete_basket_serv(book1);
    assert(service->get_basket_serv().empty());

    service->add_to_basket_serv(book1);
    bool bookNotFoundExceptionThrown = false;
    try {
        service->delete_basket_serv(invalid_book);
        assert(false && "Should have thrown BookNotFoundException");
    }
    catch (const BookNotFoundException&) {
        assert(service->get_basket_serv().size() == 1);
        bookNotFoundExceptionThrown = true;
    }
    assert(bookNotFoundExceptionThrown);

    service->delete_whole_basket();

    emptyListExceptionThrown = false;
    try {
        service->export_basket_serv(export_file);
        assert(false && "Should have thrown EmptyListException");
    }
    catch (const EmptyListException& e) {
        emptyListExceptionThrown = true;
        assert(std::string(e.what()) == "The basket is already empty");
    }
    assert(emptyListExceptionThrown);

    service->generate_basket_serv(1);
    assert(service->get_basket_serv().size() == 1);

    service->export_basket_serv(export_file);
    std::ifstream f(export_file);
    assert(f.good());
    f.close();
    std::remove(export_file.c_str());

    try {
        service->export_basket_serv(invalid_file);
        assert(false && "Should have thrown FileException");
    }
    catch (const FileException&) {
    }

    delete service;
}

void test_export_basket_failure_simple() {
    Basket basket;
    std::string export_file = "test_export.csv";

    // Empty basket should successfully create empty file
    int empty_basket = basket.export_basket(export_file);
    assert(empty_basket == 1);
    std::remove(export_file.c_str());

    Book testBook("Title", "Author", "SF", 2023, 1);
    basket.add_to_basket(testBook);
    string impossiblePath = "NUL:\\cannot_write_here.csv";
    int result = basket.export_basket(impossiblePath);
    assert(result == 0);
}

void test_file_exception_class() {
    try {
        throw FileException("Test error message");
        assert(false && "Should have thrown");
    }
    catch (const FileException& e) {
        assert(std::string(e.what()) == "Test error message");
    }
    catch (...) {
        assert(false && "Caught wrong exception type");
    }

    bool caught_as_runtime = false;
    try {
        throw FileException("Test");
    }
    catch (const std::runtime_error&) {
        caught_as_runtime = true;
    }
    assert(caught_as_runtime && "Should catch as runtime_error");
}

void test_undo_functionality() {
    // Test with InMemoryRepo
    {
        Repo* repo = new InMemoryRepo();
        test_undo_operations(repo);
        //delete repo;
    }

    // Test with FileRepo
    {
        const std::string test_file = "test_undo.txt";
        std::remove(test_file.c_str());

        Repo* repo = new FileRepo(test_file);
        test_undo_operations(repo);
        //delete repo;

        std::remove(test_file.c_str());
    }
}

void test_undo_operations(Repo* repo) {
    Service* service = new Service(repo);

    // Test Add with undo
    service->Add_serv("Book1", "Author1", "SF", 2001, 1);
    assert(service->get_list_serv().size() == 1);
    service->undo();
    assert(service->get_list_serv().empty());

    // Test Delete with undo
    service->Add_serv("Book1", "Author1", "SF", 2001, 1);
    service->Delete_serv(1);
    assert(service->get_list_serv().empty());
    service->undo();
    assert(service->get_list_serv().size() == 1);

    // Test Modify with undo
    Book original = service->get_list_serv()[0];
    service->Modify_serv("New Title", "New Author", "fantasy", 2020, 1);
    assert(service->get_list_serv()[0].get_title() == "New Title");
    service->undo();
    assert(service->get_list_serv()[0] == original);

    // Test undo stack
    try {
        service->undo();
        service->undo(); // Should throw after no more undos
        assert(false); // Shouldn't reach here
    }
    catch (const std::exception&) {
        // Expected
    }
    delete service;
    std::cout << "Undo tests passed!\n";
}

void test_load_from_file() {
    // Setup: Create a temporary file with test data
    std::ofstream test_file("test_books.txt");
    test_file << "1234|Once Upon A Broken Heart|Stephanie Garber|fantasy|2021\n";
    test_file << "1236|The Maze Runner|James Dashner|action|2009\n";
    test_file.close();

    FileRepo repo("test_books.txt");
    const auto& books = repo.get_list();

    assert(books.size() == 2);
    assert(books[0].get_id() == 1234);
    assert(books[0].get_title() == "Once Upon A Broken Heart");

    // Cleanup
    std::remove("test_books.txt");
}


void run_tests() {
    test_domain();
    test_repo_interface();
    test_file_repo_specific();
    test_in_memory_repo_specific();
    test_service();
    test_basket();
    test_service_basket_operations();
    test_export_basket_failure_simple();
    test_file_exception_class();
    test_undo_functionality();
    test_load_from_file();
    std::cout << "All tests passed successfully!\n";
}