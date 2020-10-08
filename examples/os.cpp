#include <iostream>
#include <vector>
#include <os/os.hpp>

#include <third_party/magic_enum.hpp>

#if defined(_WIN32)
    #include <winbase.h>
#endif

static constexpr const char n = '\n';

static std::string ConvertPriorityClass(const uint32_t priority)
{
    switch (priority) {
        case ABOVE_NORMAL_PRIORITY_CLASS: return "ABOVE_NORMAL_PRIORITY_CLASS";
        case BELOW_NORMAL_PRIORITY_CLASS: return "BELOW_NORMAL_PRIORITY_CLASS";
        case HIGH_PRIORITY_CLASS: return "HIGH_PRIORITY_CLASS";
        case IDLE_PRIORITY_CLASS: return "IDLE_PRIORITY_CLASS";
        case NORMAL_PRIORITY_CLASS: return "NORMAL_PRIORITY_CLASS";
        case REALTIME_PRIORITY_CLASS: return "REALTIME_PRIORITY_CLASS";
        default: return "(?)";
    }
}

int main(int, const char *[])
{
    using std::cout;
    using std::wcout;
    using std::endl;
    using std::flush;
    
    if (lavender::os::OSInformation os; os.Initialize()) {
        if (const lavender::os::OSVersionInformation &version = os.GetVersionInformation(); version.IsReady()) {
            cout << "GetBuildNumber(): " << version.GetBuildNumber() << n;
            cout << "GetVersion(): " << magic_enum::enum_name(version.GetVersion()) << n;
            cout << "GetVersionAsString(): " << version.GetVersionAsString() << n;
            cout << "GetProductType(): " << version.GetProductType() << n;
        }
        
        cout << "GetArchitecture(): " << os.GetArchitecture() << "-bit" << n;
        wcout << "GetLocale(): " << os.GetLocale() << n;

        cout << "GetComputerName():\n";
        for (const auto &name : os.GetComputerName())
            cout << ' ' << ' ' << name.first << ' ' << name.second << n;

        cout << "GetEnvironmentStrings():\n";
        for (auto &str : os.GetEnvironmentStrings())
            cout << ' ' << ' ' << str.first << " = " << str.second << n;
        
        cout << "GetRegisteredProductKey(): " << os.GetRegisteredProductKey() << n;
        cout << "IsGenuine(): " << os.IsGenuine() << n;

        if (os.TakeSnapshot(lavender::os::SnapshotType::Services | lavender::os::SnapshotType::Processes)) {
            const auto snapshot = os.GetSystemSnapshot();
            for (const auto &service : snapshot.GetServices()) {
                cout << service.GetName() << " (" << magic_enum::enum_name(service.GetType()) << ", " << magic_enum::enum_name(service.GetStatus()) << ")\n";
            }

            for (const auto &process : snapshot.GetProcesses()) {
                cout << process.GetName() << " (ID: " << process.GetID() << " (parent: " << process.GetParentID() << "), priority: " << ConvertPriorityClass(process.GetPriority()) << ", threads: " << process.GetThreadCount() << ")" << n;
                for (const auto &module : process.GetModules()) {
                    cout << "  GetName(): " << module.GetName() << n;
                    cout << "  GetImagePath(): " << module.GetImagePath() << n;
                    cout << "  GetParentID(): " << module.GetParentID() << n;
                }
                cout << n << n;
            }

            for (const auto &user : snapshot.GetUsers()) {
                wcout << L"User: " << user.GetName() << n;
                wcout << L"  IsActive() = " << user.IsActive() << n;
                wcout << L"  IsCurrentUser() = " << user.IsCurrentUser() << n;
                wcout << L"  GetFullName() = " << user.GetFullName() << n;
                wcout << L"  GetDescription() = " << user.GetDescription() << n;
                wcout << L"  GetRelativeID() = " << user.GetRelativeID() << n;
                wcout << L"  GetSID() = " << user.GetSID() << n;
                cout << "  GetPrivilege() = " << magic_enum::enum_name(user.GetPrivilege()) << n;
                cout << "  GetLastLoginTimeAsString() = " << user.GetLastLoginTimeAsString() << n;
                cout << "  GetLastLogoutTimeAsString() = " << user.GetLastLogoutTimeAsString() << n;
            }
        }
        
        cout << "GetPaths(): " << n;
        for (const auto &path : os.GetPaths()) {
            cout << magic_enum::enum_name(path.first) << " = ";
            wcout << path.second << n;
        }
        cout << n;

        cout << flush;
    }
}

