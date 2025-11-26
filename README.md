# Kernel-Mode Memory Driver for Game Hacking (User-Mode Bypass Only)

⚠️ Warning  
This project is intended for **educational purposes only**. If you are not experienced with Windows internals, kernel development, or system programming, **do not run this driver**. Improper use or misunderstanding of kernel‑level code can result in system crashes (BSODs), security risks, or permanent system instability.  
Running unsigned kernel‑mode drivers can be dangerous if you don’t know exactly what you’re doing.

📌 Summary  
This is a basic Windows kernel‑mode memory driver project for interfacing with the memory of a target process. It enables user‑mode applications to:  
- Attach to a process (by PID)  
- Read memory  
- Write memory  

The project is designed with learning and testing in mind. It’s tailored for games with minimal or no anti‑cheat protection (such as AssaultCube), and serves as an entry point into kernel‑level game hacking.

❗ This driver **only bypasses user‑mode anti‑cheat systems**. It **does not** bypass kernel‑mode anti‑cheats (like BattleEye or EAC).

💡 What This Driver Is  
- A **basic kernel‑mode driver** written in C++  
- Implements **DeviceIoControl‑based communication** between user‑mode and kernel‑mode  
- Supports a **custom protocol** for attaching to processes and performing memory operations  
- Uses **MmCopyVirtualMemory** for safely transferring memory between processes  

This design allows a user‑mode cheat to interface with the driver and read/write memory in the target process **from the kernel context**, bypassing any basic user‑mode protections.

🔧 Included Headers & Features

✅ driver_helper.h  
Contains a lightweight wrapper class ioctl::Driver to interface with the kernel driver using DeviceIoControl.  

Functions: 
```cpp
attach_to_process(DWORD pid) // Sends the IOCTL attach code to the driver with the target PID  
read_memory<T>(uintptr_t address) // Template function to read memory from the attached process  
write_memory<T>(uintptr_t address, const T& value) // Template function to write memory into the attached process  
```
All of these internally use DeviceIoControl with the custom IOCTL codes (read, write, attach).

✅ error_helper.h  
Contains a utility function:  
```cpp
std::string GetErrorString(DWORD errorCode)  
```
Converts a Win32 error code to a human‑readable string using FormatMessageA. Useful for debugging WinAPI failures.

✅ offsets.h  
Basic namespace for storing memory offsets:  
```cpp
namespace offsets  
{  
    ::uintptr_t exampleOffset = 0xEC;  
}
```
You can define your own offsets here for use with read_memory and write_memory.

✅ process_helper.h  
Class Process wraps around some WinAPI calls to interact with a target process.

Constructor: 
```cpp
Process(const char* name) // Finds the PID of the process and stores it in pid. Also opens a handle with OpenProcess.
```

Functions:  
```cpp
bool GetModules() // Collects all loaded modules of the process and fills them into the "modules" map to allow easy module indexing.
void PrintAllModules(bool print_addresses = false) // Prints module names and optionally base addresses  
```
Destructor:  
Closes the process handle.

Members:
```cpp
std::map<std::string, uintptr_t> modules // Contains module names and their base addresses for easy offset calculation and module indexing.
```

🚀 Example Use  
After loading the driver and ensuring it’s running, in your user‑mode client you might write:
```cpp
Process process("ExampleProcessName.exe");  

uintptr_t baseAddress = process.modules["Game.exe"];
uintptr_t localPlayer = driver.read_memory<uintptr_t>(baseAddress + offsets::exampleOffset);  
int health = driver.read_memory<int>(localPlayer + offsets::exampleOffset);
```
✅ Tested On  
- Windows 10 VM (VirtualBox)  
- Test target: Dummy application  
- Cheat validated using DebugView + user‑mode client

⚙️ Requirements  
- Visual Studio (with Windows Driver Kit – WDK)  
- Signed driver loading or test mode (bcdedit /set testsigning on)  
- Administrator privileges to load .sys drivers  
- DebugView (SysInternals) for kernel print logging (DbgPrint)

❗ Final Notes  
- Always test in a **virtual machine**.  
- This is a starting point for **learning**, not a finished cheat.  
- No kernel anti‑cheat bypass is implemented. Use only in controlled environments.
