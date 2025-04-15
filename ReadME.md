
# Student Record Management System
![IMAGE](https://github.com/DhairyaDave08/efficient-data-storage-and-retrieval-system/blob/main/images/B-Tree-logo.png)
A **C++ based Student Record Management System** featuring:

- Password-protected authentication
- B-Tree based record management
- File I/O for persistence
- CSV export for external use

---

## 🔧 Features

### 1. User Authentication
- Secure login with a password prompt
- Lockout after multiple failed attempts

### 2. B-Tree Implementation
- Efficient insertions, deletions, and searches
- Maintains sorted order of student records
- Scalable for large datasets

### 3. Record Management
- Add, search, and delete student data
- Export records to `student_records.csv`

---

## 🧠 Data Structures Used

- **B-Tree**: Core structure used for indexing and managing student records
- **File Streams**: For reading/writing persistent data
- **Arrays and Strings**: Used in basic data handling

---

## 🧱 OOP Concepts Applied

- **Classes and Objects**: Student and B-Tree nodes are modeled as classes
- **Encapsulation**: Data members are kept private and accessed via methods
- **Constructor/Destructor**: Used to manage resource initialization and cleanup
- **File Handling**: Member functions handle reading/writing to files
- **Static Members**: Optional (can be used for tracking counts or defaults)

---

## 📂 File Structure

- `main.cpp` — Main program logic and menu
- `student_data.txt` — Persistent storage of student records
- `student_records.csv` — Exported student data for external use

---

## 🚀 How to Run

1. **Compile the program**
   ```bash
   g++ main.cpp -o student_system
   ```

2. **Run the executable**
   ```bash
   ./student_system
   ```

3. **Login**
   Enter the password when prompted (default is hardcoded in the code).

---

## 📝 Record Format

Each student record contains:
- Roll No (Integer)
- Name (String)
- Last Name (String)
- Course (String)
- Hostel Name (String)

Example:
```
Roll No: 101
Name: Ravi
Last Name:Bhatt
Course:ICT
Hostel Name-XYZ
```

---
## 🖼️ Screenshot

![Screenshot](https://github.com/DhairyaDave08/efficient-data-storage-and-retrieval-system/blob/main/images/B-Tree-screenshot.png)

## 🔮 Future Plans

- Getline function should be used instead of cin for storing strings 
- Use **password hashing** for secure authentication.
- Implement a **graphical user interface (GUI)** with Qt or other libraries
- Could add sort function so that the user find easier method to search

---

## 📃 License

This project is open source under the [MIT License](LICENSE).



