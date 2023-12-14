//forward declarations

class Team;
class WorkRequest;
class TeamMember;
class FileHandler;
class dataManager;
class TeacherInterface;
class Teacher;
class  TeamInterface;
class Adminitrator;

#include <iostream>
#include <ctime>
#include<fstream>
#include <vector>
#include<string>
#include<cstring>
#include <sstream>


using namespace std;

const int MAX_TEAM_MEMBERS = 10;
const int MAX_TEAM = 10;
const int MAX_SERVICE_REQUESTS = 10;





class WorkRequest
{
public:
    int id;
    string description;
    string status;
    time_t date;
    string feedback;
    string teacherName;
    vector<int> teamMemberAssigned;

    WorkRequest(int wid = 0, const string& desc = "None", string statuss = "new", string teacherNames="None",  time_t dates = time(0), string feedback = "None", const vector<int>&ass = {})
        : id(wid), description(desc), teacherName(teacherNames), status(statuss), date(dates) {
        teamMemberAssigned.assign(ass.begin(), ass.end());
    }


    void updateStatus(const string& newStatus)
    {
        status = newStatus;
    }
    string getstatus() const { return status; }
    //WorkRequest(const WorkRequest& other) {
    //    this->id = other.id;
    //    this->description = other.description;
    //    this->status = other.status;
    //    this->date = other.date;
    //    this->feedback = other.feedback;
    //    this->teacherName = other.teacherName;
    //    this->teamMemberAssigned = other.teamMemberAssigned; // Assuming vector has a copy constructor
    //}
    void addFeedback(const string& userFeedback)
    {
        feedback = userFeedback;
    }

    void requestteamMemberAssigned(int teamMemberId)
    {
        int chk = 0;
        for (int i = 0; i < teamMemberAssigned.size(); ++i)
        {
            if (teamMemberAssigned[i]==teamMemberId)
            {
                chk = 1;
            }
        }
        if(chk==0) teamMemberAssigned.push_back(teamMemberId);
        updateStatus("assigned");
    }

    void completeTask()
    {
        updateStatus("resolved");
    }
    void showdetails() const{
        cout << endl << "complaintID: " << this->id << "\tDiscription: " << this->description << "\tDate: " << this->date << "\tStatus: " << this->status << "\tSubmitted By: " << this->teacherName;

    }
    ~WorkRequest() {};
};
class TeamMemberInterface {
public:
    virtual void Work() = 0;
    virtual void updatestatus() = 0;
    virtual void MemberLogin() = 0;
    virtual void MemberLogOut() = 0;

    virtual void TeamMemberLogin(vector<TeamMember> members) = 0;
};
class TeamMember: public TeamMemberInterface
{
public:
    
    int id;
    int MemberLoggedIn;
    string name;

    vector<WorkRequest> jobs;
    
    TeamMember(int memberId = 0, const string& memberName = "", vector<WorkRequest> jobs = {}) : id(memberId), name(memberName), jobs(jobs) { this->MemberLoggedIn = 0; /*cout <<"\nMember created with name "<< memberName << " ID:  " << memberId << endl;*/ }
    
    void Work() {
       // if (MemberLoggedIn) {
            cout << "\n These are your assigned jobs: " << endl;
            for (int i = 0; i < jobs.size(); i++) {
                cout << endl << " complaintID: " << jobs[i].id << " Discription: " << jobs[i].description << " Date: " << jobs[i].date << " Status: " << jobs[i].status;
            }
            if (jobs.size() == 0) { cout << "\n YOU HAVE NO WORK REQUESTS\n "; }
           
       
    }
    int getid() const { return this->id; }
    string getname() const { return this->name; }
    //TeamMember(const TeamMember& other) {
    //    this->id = other.id;
    //    this->MemberLoggedIn = 0;
    //    this->name = other.name;
    //    this->jobs = other.jobs; // Assuming WorkRequest has a copy constructor or copy assignment operator
    //}
    void updatestatus() {
        if (MemberLoggedIn) {

            Work();
            cout << endl << " please Enter ID which request you would like to update the status of: ";
            int ID;
            cin >> ID;
            int chk = 0;
            for (int i = 0; i < jobs.size(); i++) {
                if (jobs[i].id == ID) {
                    chk = 1;
                    jobs[i].updateStatus("Complete");
                }
            }
            if (!chk) { cout << "\n NO SUCH WORK REQUEST FOUND \n"; }
        }
        else {
            cout << "\n PLEASE LOG IN FIRST \n";
        }
    }
    void MemberLogin() { this->MemberLoggedIn = 1; }
    void MemberLogOut() { this->MemberLoggedIn = 0; }
    void TeamMemberLogin(vector<TeamMember> members) {
        cout << "\n WELCOME TeamMember \n" << endl << " Please LOGIN by Entering your Name and id  " << endl << " Name: ";
        string manName; int manid;
        cin >> manName;
        cout << endl << " ID: ";
        cin >> manid;
        for (const auto& m : members) {
            if (m.id == manid && m.name == manName) { this->MemberLoggedIn = 1; cout << "\n WELCOME " << manName << endl; }
        }

        
    }
    void showjobs() {
        for (int i = 0; i < jobs.size(); i++) {
            cout << endl << " complaintID: " << jobs[i].id << " Discription: " << jobs[i].description << " Date: " << jobs[i].date << " Status: " << jobs[i].status<<endl;
        }
    }

};

class TeamInterface
{
public:
    void virtual viewWorkRequests(vector<TeamMember> members) = 0;
    void virtual assignTask() = 0;
    void virtual completeTask() = 0;
    void virtual viewTeamMembers() = 0;
    virtual void ManagerLogin() = 0;
    virtual void ManagerLogOut() = 0;

};

class Team: public TeamInterface
{
public:
    int ManagerID;
    int ManagerLoggedIn;
    string name;
    string ManagerName;//Actually the managers name
    vector<TeamMember> teamMembers;
    vector<WorkRequest> workRequests;


    Team(const string& teamName="", string ManagerNames = "no name", int ManagerIDs = 0, const vector<TeamMember>& members = {}, const vector<WorkRequest>& requests = {})
        : name(teamName) {
        ManagerName = ManagerNames;
        ManagerID = ManagerIDs;
        ManagerLoggedIn = 0;
        teamMembers.assign(members.begin(), members.end());
        workRequests.assign(requests.begin(), requests.end());
      

    }
    void ManagerLogin() { this->ManagerLoggedIn = 1; }
    void ManagerLogOut() { this->ManagerLoggedIn = 0; }
    void assignTask() {
        if (ManagerLoggedIn) {
            
            viewTeamMembers();
            int serviceRequestId; int teamMemberId;
            cout << endl << " Please enter the requestID and the teamMemberID to assign that particular Member to that id" << endl;
            cout << "Enter serviceRequestId : "; cin >> serviceRequestId;
            cout << "Enter teamMemberId : "; cin >> teamMemberId;
            if (this->assignTeamMember(serviceRequestId, teamMemberId))
                cout << "Task assigned successfully for Service Request ID: " << serviceRequestId << " and TeamMemberID :" << teamMemberId << endl;
            else
                cout << "Can not assign task for serviceRequestId : " << serviceRequestId << " and TeamMemberID :" << teamMemberId << endl;

        }
    }
    void completeTask()
    {
        if (ManagerLoggedIn) {

            //viewWorkRequests();
            int serviceRequestId;
            cout << endl << " Please ENTER any workRequestID that you Would like to Assign complete : ";
            cin >> serviceRequestId;
            for (int i = 0; i < workRequests.size(); ++i)
            {
                if (workRequests[i].id == serviceRequestId)
                {
                    workRequests[i].updateStatus("Resolved");
                    cout << "Task completed successfully for Service Request ID: " << serviceRequestId << endl;

                    break;
                }
            }
            cout << "\n No such workRequest found ";
        }
    }
    void viewWorkRequests(vector<TeamMember> members) {
        if (ManagerLoggedIn) {
            competedWorkRequestsByTeam(members);
            for (auto& workrequest : workRequests) {
                workrequest.showdetails();
             }
            
        }
        
    }
    void viewTeamMembers() {
        if (ManagerLoggedIn) {

            for (auto it = this->teamMembers.begin(); it != this->teamMembers.end(); ++it) {
                TeamMember& teamMember = *it;
                cout << "Team Member ID: " << teamMember.id << "\tName: " << teamMember.name << endl;
                teamMember.showjobs();
            }
        }
    }
    void showtamDetails() {
        if (ManagerLoggedIn) {

            cout << "\n Team Name: " << this->name << "\n Manager Name: " << this->ManagerName << "\n Manager Id: " << this->ManagerID << endl << "These are the team members \n";
            viewTeamMembers();
            cout << endl << " These are the requests assigned to this team \n";
            //viewWorkRequests();
        }
    }
    void setManagerName(string manName){
        this->ManagerName = manName;
    }
    void setManagerid(int manName) {
        this->ManagerID = manName;
    }
    //notify
    void newWorkRequestsSubmitted() {
        if (ManagerLoggedIn) {

            cout << " New Work Requests in " << this->name << " team:" << endl;
            for (auto it = this->workRequests.begin(); it != this->workRequests.end(); ++it)
            {
                WorkRequest& workRequest = *it;
                if (workRequest.getstatus() == "new") {
                    workRequest.showdetails();
                }
            }

        }
    }
    //notify
    void competedWorkRequestsByTeam(vector<TeamMember> members) {
        if (ManagerLoggedIn) {

            cout << " competed Work Requests in " << this->name << " by team Members:" << endl;
            for (auto& allmembers : members) {
                for (auto it = this->teamMembers.begin(); it != this->teamMembers.end(); ++it)
                {
                    TeamMember member = *it;
                    if (allmembers.getid() == member.getid() && allmembers.getname() == member.getname()) {
                        for (const auto& workRequest : allmembers.jobs)
                        {
                            if (workRequest.getstatus() == "Complete") {
                                workRequest.showdetails();
                            }

                        }
                    }
                }
            }
          


        }
    }
    //helper function
    bool assignTeamMember(int serviceRequestId, int teamMemberId)
    {
        if (ManagerLoggedIn) {

            int chk = 0;
            for (int i = 0; i < workRequests.size(); ++i)
            {
                if (workRequests[i].id == serviceRequestId)
                {
                    workRequests[i].requestteamMemberAssigned(teamMemberId);
                    chk = 1;
                }
            }
            if (chk == 0) { cout << endl << "workRequest does not exists"; return false; }
            for (int i = 0; i < teamMembers.size(); ++i)
            {
                if (teamMembers[i].id == teamMemberId && chk == 1)
                {
                    teamMembers[i].jobs.push_back(workRequests[i]);
                    return true;
                }
            }
        }
        return false;

    }

   
};


class TeacherInterface
{
public:
    virtual void submitWorkRequest(vector<Team>& team) = 0;
    virtual void provideFeedback( vector<Team>& team) = 0;
    virtual void showRequests() = 0;
    virtual void TeacherLogin() = 0;
    virtual void TeacherLogOut() = 0;
};

class Teacher : public TeacherInterface{
  
public:
    int id;
    string name;
    int teacherLoggedIn;
    vector<WorkRequest> teacherRequest;

    Teacher(int id, string name, vector<WorkRequest> teacherRequests={}) : id(id), name(name) {
        this->teacherLoggedIn = 0;
        teacherRequest.assign(teacherRequests.begin(), teacherRequests.end());
    }
    void showTeacherDetails() {
       
            cout << "\n Name:" << this->name << " id:" << this->id << endl;
            cout << " These are the request " << this->getname() << " has made : " << endl;
            for (int i = 0; i < this->teacherRequest.size(); i++) {
                teacherRequest[i].showdetails();
            }
        
    }
    void TeacherLogin() { this->teacherLoggedIn = 1; }
    void TeacherLogOut() { this->teacherLoggedIn = 0; }
    string getname() const {return this->name;}
    int getid() const {return this->id;}
    void   submitWorkRequest( vector<Team>& teams) {
        if (teacherLoggedIn) {
            cout << "\nPLEASE ENTER THE PROBLEM YOU ARE FACING" << endl;
            string description;
            cin >> description;
            cout << "\nPLEASE ENTER TO WHICH NAME OF TEAM YOU WOULD LIKE TO SUBMIT THIS REQUEST TO" << endl;
            string TeamName;
            cin >> TeamName;
            auto teamIt = teams.begin();
            while (teamIt != teams.end() && teamIt->name != TeamName) {
                ++teamIt;
            }

            if (teamIt != teams.end()) {
                int serviceRequestId = teamIt->workRequests.size() + 1;
                WorkRequest request(serviceRequestId, description);
                teamIt->workRequests.push_back(request);
                this->teacherRequest.push_back(request);
                cout << "Work request submitted successfully. Service Request ID: " << serviceRequestId << std::endl;
                // teamIt->Team.newRequests(serviceRequestId);
            }
            else {
                std::cout << "Team not found: " << TeamName << std::endl;
            }
        }
    }


    void provideFeedback(  vector<Team>& teams) {
        if (teacherLoggedIn) {
            showRequests();

            cout << "\nPLEASE ENTER THE ID OF THE REQUEST YOU WANT TO GIVE FEEDBACK TO " << endl;
            int serviceRequestId;
            cin >> serviceRequestId;
            cout << "\nPLEASE ENTER THE FEEDBACK YOU WOULD LIKE TO SUBMIT IN THIS REQUEST" << endl;
            string feedback;
            cin >> feedback;
            for (auto& team : teams) {
                for (auto& workRequest : team.workRequests) {
                    if (workRequest.id == serviceRequestId) {
                        workRequest.addFeedback(feedback);

                        cout << "Feedback recorded successfully for Service Request ID: " << serviceRequestId << endl;
                    }
                }
            }
            for (auto& Request : teacherRequest) {
                if (Request.id == serviceRequestId) {

                    Request.addFeedback(feedback);

                    return;
                }
            }

            cout << "Service request not found with ID: " << serviceRequestId << endl;
        }
    }
    void showRequests() {
        if (teacherLoggedIn) {
            cout << "\nthese are the requests you have already submitted: " << endl;
            for (auto& Request : teacherRequest) {
                Request.showdetails();

            }
        }
    }
};
class FileHandler
{
public:
    //reading from files
    static vector<Team> readTeamsFromFile(const string& filename, vector<Team>& teams, vector<TeamMember>& teamMembers) {
        ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            perror("Error");

            return {};
        }

        // Vector to store read Team objects
        string teamName = "";
        int numMembers = 0; int numRequests = 0; int ManagerID = 0; string ManagerName = "";
        int teamsize;
        inFile >> teamsize;
        for (int k = 0; k < teamsize;k++) {
            inFile >> teamName >> numMembers >> numRequests;
            vector<TeamMember> members;
            vector<WorkRequest> requests;


            // Read team Team's name
            inFile >> ManagerID >> ManagerName;



            // Read team members' information
            for (int i = 0; i < numMembers; ++i) {
                int id; string name;
                inFile >> id >> name;
                TeamMember member(id, name);
                members.push_back(member);
                //teamMembers.push_back(member);
            }

            for (int i = 0; i < numRequests; i++) {
                int id; string description, teacherName, status, feedback; time_t date;

                inFile >> id >> description >> teacherName >> status >> date >> feedback;

                // Read task teamMemberAssigned
                vector<int> teamMemberAssigned;
                int tempId = 0;

                    inFile >> tempId;
                    teamMemberAssigned.push_back(tempId);

                

                WorkRequest request(id, description, status, teacherName, date, feedback, teamMemberAssigned);

                for (int i = 0; i < request.teamMemberAssigned.size(); i++) {
                    for (auto& m : teamMembers) {
                        if (m.getid() == request.teamMemberAssigned[i]) {
                            m.jobs.push_back(request);  
                            break; }
                    }
                }
                requests.push_back(request);  // Store the WorkRequest object in the array
            }

            Team team(teamName, ManagerName, ManagerID, members, requests);
            teams.push_back(team);
        }

        inFile.close();
        return teams;
    }
    static vector<TeamMember> readTeamMemberFromFile(const string& filename, vector<TeamMember>& members) {
        ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            perror("Error");

            return {};
        }
        int totalMembers = 0;
        inFile >> totalMembers;
        for (int i = 0; i < totalMembers; i++) {
            string memberName;
            int memberID;
            inFile >> memberID >> memberName;
            int chk = 0;
            for (const auto& m : members) {
                if (m.name == memberName && m.id == memberID) { chk = 1; break; }
            }if (chk == 0) {
                TeamMember member(memberID, memberName);
                members.push_back(member);
            }
        }
        inFile.close();
        return members;
    }
    static vector<Teacher> readTeacherFromFile(const string& filename, vector<Teacher>& teachers) {
        ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            perror("Error");

            return {};
        }
        int totalTeachers;
        string memberName;
        int memberID, numRequests;
        vector<WorkRequest> requests;

        inFile >> totalTeachers;
        for (int i = 0; i < totalTeachers; i++) {

            inFile >> memberID >> memberName;


            inFile >> numRequests;
            for (int i = 0; i < numRequests; i++) {
                int id; string description, teacherName, status, feedback; time_t date;

                inFile >> id >> description >> teacherName >> status >> date >> feedback;





                WorkRequest request(id, description, status , teacherName, date, feedback);
                requests.push_back(request);
            }

            Teacher member(memberID, memberName, requests);
            teachers.push_back(member);
        }
        inFile.close();
        return teachers;
    }

    //writing to files
    static void writeTeamsToFile(const vector<Team>& teams, const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        outFile << teams.size()<<endl;
        for (const auto& team : teams) {
            outFile << team.name << endl << team.teamMembers.size() << endl << team.workRequests.size() << endl;
            outFile << team.ManagerID << " " << team.ManagerName << endl;
            // Write team members' information
            for (const auto& member : team.teamMembers) {
                outFile << member.id << " " << member.name << endl;
            }

            // Write work requests' information
            for (const auto& request : team.workRequests) {
                outFile << request.id << " " << request.description << " " << request.status << " " << request.date << " " << request.feedback;

                for (const auto& assignment : request.teamMemberAssigned) {
                    outFile << " " << assignment;
                }

                outFile << endl;
            }
        }

        outFile.close();
    }
    static void writeTeamMemberToFile(const vector<TeamMember>& members, const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            perror("Error");

            return;
        }
        outFile << members.size() << endl;
        for (const auto& member : members) {
            outFile << member.id << " " << member.name << endl;;

        }


    }
    static void writeTeacherToFile(vector<Teacher>& teachers, const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            perror("Error");

            return;
        }
        outFile << teachers.size() << endl;
        for (const auto& teacher : teachers) {
            outFile << teacher.id << " " << teacher.name << endl;

            outFile << teacher.teacherRequest.size() << endl;
            for (int i = 0; i < teacher.teacherRequest.size(); i++) {

                outFile << teacher.teacherRequest[i].id <<" "<< teacher.teacherRequest[i].description << " " << teacher.teacherRequest[i].teacherName << " " << teacher.teacherRequest[i].status << " " << teacher.teacherRequest[i].date <<" "<< teacher.teacherRequest[i].feedback << endl;
            }


        }


    }

};

class dataManager
{
public:

    void loadData(vector<Team>& teams, vector<TeamMember>& members, vector<Teacher>& teachers)
    {
        // Load data from files and assign it to the teams array
        string filename = "team.txt";
        FileHandler::readTeamsFromFile(filename, teams, members);
        string filename1 = "member.txt";
        FileHandler::readTeamMemberFromFile(filename1, members);
        string filename2 = "teacher.txt";
         FileHandler::readTeacherFromFile(filename2, teachers);

    }

    void saveData(vector<Team>& teams, vector<TeamMember>& members, vector<Teacher>& teachers)
    {//writing in team1 for now to try
        FileHandler::writeTeamsToFile(teams, "team.txt");
        FileHandler::writeTeamMemberToFile(members, "member.txt");
        FileHandler::writeTeacherToFile(teachers, "teacher.txt");
    }
};
class AdministratorInterface {
public:
    virtual void viewWorkRequestsSummary() = 0;
    virtual void viewTeamWorkRequests() = 0;
    virtual void addTeacher() = 0;
    virtual void addTeamMember() = 0;
    virtual void showTeamMembers() = 0;
    virtual void DeleteTeamMember() = 0;
    virtual void showTeams() = 0;
    virtual void DeleteTeam() = 0;
    virtual void showTeachers() = 0;
    virtual void DeleteManager() = 0;
    virtual void AddManager() = 0;
    virtual void AdminLogin(int adminid,string adminname) = 0;
    virtual void AdminLogout(vector<Team>& team, vector<TeamMember>& teamMember, vector<Teacher>& teacher) = 0;
    // Add other pure virtual functions as needed...
};
//SINGLETON CLASS
class Administrator : public AdministratorInterface {
private:
    static Administrator* instance;

    Administrator(int adminid = 0, string adminname = "Admin", const vector<Team>& team = {}, const vector<Teacher>& teacher = {}, const vector<TeamMember>& teamMember = {});
    

public:
    static Administrator* getInstance(int id = 0, string name = "Admin", const vector<Team>& team = {}, const vector<Teacher>& teacher = {}, const vector<TeamMember>& teamMember = {}) {
        if (instance == nullptr) {
            instance = new Administrator(id,name,team,teacher, teamMember);
        }
        return instance;
    }
    int id;
    int adminLoggedIn;
    string name;
    vector<Team> teams;
    vector<Teacher> teachers;
    vector<TeamMember> teamMembers;

    

    void AdminLogin(int adminid,string adminName) {
        if (adminid == this->id && adminName == this->name) { adminLoggedIn = 1; }
    }
    void AdminLogout( vector<Team>& team , vector<TeamMember>& teamMember ,  vector<Teacher>& teacher ) {
        adminLoggedIn = 0;
        dataManager save;
        save.saveData(this->teams, this->teamMembers, this->teachers);
        save.loadData(team, teamMember, teacher);
    }
    void viewWorkRequestsSummary()
    {
        if (adminLoggedIn) {
            time_t startDate; time_t endDate;
            int sec = 0;
            cout << "\nTHE START DATE WILL BE of THE EARLIEST WORK REQUEST ALREADY PRESENT IN THE SYSTEM \n";
            startDate = 1637842850;
            cout << "\nPlease Enter the amount of seconds before the current time you want to display the requests of \n";
            cin >> sec;
            endDate = time(0) - sec;
            cout << "Work Requests summary from " << startDate << " to " << endDate << ":" << endl;

            for (int i = 0; i < this->teams.size(); ++i)
            {
                const auto& team = this->teams[i];
                cout << "\nTeam: " << team.name << endl;

                for (const auto& workRequest : team.workRequests)
                {
                    if (workRequest.date >= startDate && workRequest.date <= endDate)
                    {
                        cout << "Service Request ID: " << workRequest.id << "\tStatus: " << workRequest.status << "   Discription: " << workRequest.description << "   Date: " << workRequest.date << endl;
                    }
                }
            }
        }
    }
    void viewTeamWorkRequests()
    {
        if (adminLoggedIn) {

            cout << endl << "\n Please enter the name of the team for which you want to view the requests of ";
            string teamName;
            cin >> teamName;
            for (const auto& team : this->teams)
            {
                if (team.name == teamName)
                {
                    cout << "Work Requests in " << team.name << " team:" << endl;
                    for (const auto& workRequest : team.workRequests)
                    {
                        workRequest.showdetails();
                    }
                    return;
                }
            }

            cout << "Team not found: " << teamName << endl;
        }
    }
    void addTeacher() {
        showTeachers();
        string teachername;
        cout << "\n Please Enter the name of Teacher you want to add ";
        cin >> teachername;


        for (const auto& teacher : this->teachers)
        {
            if (teacher.name == teachername)
            {

                cout << "\n Teacher already exists ";
                return;

            }
        }
        int teacherid = this->teachers.size();
        Teacher t(teacherid, teachername);
        this->teachers.push_back(t);
        cout << "\n Teacher successfully added\n ";
        cout << "\n auto genrated id of teacher is :" << teacherid;

    }
    void addTeamMember() {
        if (adminLoggedIn) {

            showTeamMembers();
            string memberName;
            cout << "\n Please Enter the name of team member you want to add ";
            cin >> memberName;


            for (const auto& member : this->teamMembers)
            {
                if (member.name == memberName)
                {

                    cout << "\n Member already exists ";
                    return;
                }
            }
            int memberid = this->teamMembers.size();
            memberid++;
            TeamMember member(memberid, memberName);
            this->teamMembers.push_back(member);
            cout << "\n Member successfully added\n ";
            cout << "\n auto genrated id of Member is :" << memberid;

        }
    }
    void showTeamMembers() {
        if (adminLoggedIn) {

            for (auto it = this->teamMembers.begin(); it != this->teamMembers.end(); ++it) {
                TeamMember& teamMember = *it;
                cout << "Team Member ID: " << teamMember.id << "\tName: " << teamMember.name << endl;
            }
        }
    }
    void DeleteTeamMember() {
        if (adminLoggedIn) {
            string memberName, teamName;
            showTeamMembers();

            cout << "\n Please Enter the name of team member you want to DELETE :";
            cin >> memberName;
            cout << "\n Please Enter the Team name where he will be Deleted from specificallly:  ";
            cin >> teamName;

            for (const auto& member : this->teamMembers)
            {
                auto memberIt = std::remove_if(
                    this->teamMembers.begin(),
                    this->teamMembers.end(),
                    [memberName](const TeamMember& member) { return member.name == memberName; });

                if (memberIt != this->teamMembers.end())
                {
                    this->teamMembers.erase(memberIt);
                    cout << "\n Member Deleted ";
                    break;
                }
                else
                {
                    // cout << "\n Member not found ";
                }
            }
            for (const auto& team : this->teams)
            {
                for (const auto& member : team.teamMembers)
                {
                    auto memberIt = std::remove_if(
                        this->teamMembers.begin(),
                        this->teamMembers.end(),
                        [memberName](const TeamMember& member) { return member.name == memberName; });

                    if (memberIt != this->teamMembers.end())
                    {
                        this->teamMembers.erase(memberIt);
                        cout << "\n Member Deleted ";
                    }
                    else
                    {
                        // cout << "\n Member not found ";
                    }
                }
            }

        }
    }
    void showTeams() {
        if (adminLoggedIn) {

            for (auto& team : teams) {
                team.showtamDetails();
            }
        }
    }
    void DeleteTeam()
    {
        if (adminLoggedIn) {

            string teamName;
            showTeams();

            cout << "\n Please Enter the name of the team you want to DELETE ";
            cin >> teamName;

            // Erase the team
            for (const auto& team : this->teams)
            {
                auto teamIt = std::remove_if(
                    this->teams.begin(),
                    this->teams.end(),
                    [teamName](const Team& team) { return team.name == teamName; });

                if (teamIt != this->teams.end())
                {
                    this->teams.erase(teamIt);
                    cout << "\n Team Deleted ";
                }
                else
                {
                    cout << "\n Team not found ";
                }
            }
        }
    }
    void showTeachers() {
        if (adminLoggedIn) {

            cout << "\n these are the Teachers present \n";
            for (auto& teacher : teachers) {
                teacher.showTeacherDetails();
            }
        }
    }
    void DeleteTeacher()
    {
        if (adminLoggedIn) {

            string teacherName;
            this->showTeachers();

            cout << "\n Please Enter the name of the teacher you want to DELETE ";
            cin >> teacherName;

            // Erase the teacher
            for (const auto& teacher : this->teachers) {
                string name = teacher.getname();
                auto teacherIt = std::remove_if(
                    this->teachers.begin(),
                    this->teachers.end(),
                    [teacherName](const Teacher& teacher) { return  teacher.name == teacherName; });

                if (teacherIt != this->teachers.end())
                {
                    this->teachers.erase(teacherIt);
                    cout << "\n Teacher Deleted ";
                }
                else
                {
                    cout << "\n Teacher not found ";
                }
            }
        }
    }
    void DeleteManager(){
        {
            if (adminLoggedIn) {

                string teacherName;
                this->showTeams();

                cout << "\n Please Enter the name of the MANAGER you want to DELETE ";
                cin >> teacherName;

                // Erase the teacher
                for (const auto& team : this->teams) {
                    

                    if (team.ManagerName== teacherName)
                    {
                       
                        cout << "\n Manager Deleted ";
                    }
                    else
                    {
                        cout << "\n Manager not found ";
                    }
                }
            }
        }
    }
    void AddManager() {
        cout << "add Manager ";
    }
   
};
Administrator* Administrator::instance = nullptr;
Administrator::Administrator(int adminid, string adminname, const vector<Team>& team , const vector<Teacher>& teacher , const vector<TeamMember>& teamMember ) {
    this->teams.assign(team.begin(), team.end());
    this->teachers.assign(teacher.begin(), teacher.end());
    this->teamMembers.assign(teamMember.begin(), teamMember.end());
    this->id = adminid;
    this->name = adminname;
    this->adminLoggedIn = 0;
}



int main()
{
    vector<Team> teams;
    vector<TeamMember> members;
    vector<Teacher> teachers;
    dataManager dataManager;
    dataManager.loadData(teams, members, teachers);

    // Example with a vector of Team objects
    while (1)


    {
        cout << " \t\t WELCOME \t\t \n";
        cout << "WHO DO YOU WANT TO LOGIN AS\n";
        cout << " [1] TEAM MEMBER " << endl << " [2] MANAGER " << endl << " [3] TEACHER" << endl << " [4] ADMINISTRATOR "<<endl<<" [0] TO QUIT\n";
        int choice = 0;
        cin >> choice;

        if (choice == 1) {
            cout << "\n WELCOME TeamMember \n" << endl << " Please LOGIN by Entering your Name and id  " << endl << " Name: ";
            string manName;
            int manid;
            cin >> manName;
            cout << endl << " ID: ";
            cin >> manid;
            int chk = 0;

            TeamMemberInterface* NewMember = nullptr;
            int memberIndex = -1;

            for (int i = 0; i < members.size(); ++i) {
                if (members[i].id == manid && members[i].name == manName) {
                    memberIndex = i;
                    chk = 1;
                    cout << "\n WELCOME " << members[i].getname() << endl;
                    break;
                }
            }

            if (!chk) {
                cout << "\n NO SUCH MEMBER EXISTS PLEASE ASK THE ADMINISTRATOR TO ADD YOU ";
            }
            else {
                // Now, NewMember points to the original object in the vector
                NewMember = &members[memberIndex];
                NewMember->MemberLogin();
               
                while (1) {
                    cout << "\n waht would you like to do today " << manName << endl;
                    cout << " [1] SEE WORK REQUESTS FOR YOU " << endl << " [2] COMPLETE SPECIFIC WORK REQUESTS " << endl << " [3] LOGOUT" << endl;
                    int Memberchoice = 0;
                    cin >> Memberchoice;
                    if (Memberchoice == 1) {
                        NewMember->Work();
                    }
                    if (Memberchoice == 2) {
                        NewMember->updatestatus();
                    }
                    if (Memberchoice == 3) {
                        NewMember->MemberLogOut();
                        cout << "\n\n\t\t BYE BYE\t\t\n\n";
                        break;
                    }
                }

            }
        }


        else if (choice == 2) {
            {
                cout << "\n WELCOME Manager \n" << endl << " Please LOGIN by Entering your Name and id  " << endl << " Name: ";
                string manName;
                int manid;
                cin >> manName;
                cout << endl << " ID: ";
                cin >> manid;
                int chk = 0;

                TeamInterface* NewManager = nullptr;
                int memberIndex = -1;

                for (int i = 0; i < teams.size(); ++i) {
                    if (teams[i].ManagerID == manid && teams[i].ManagerName == manName) {
                        memberIndex = i;
                        chk = 1;
                        cout << "\n WELCOME " << manName << endl;
                        break;
                    }
                }

                if (!chk) {
                    cout << "\n NO SUCH MANAGER EXISTS PLEASE ASK THE ADMINISTRATOR TO ADD YOU ";
                }
                else {
                    // Now, NewMember points to the original object in the vector
                    NewManager = &teams[memberIndex];
                    NewManager->ManagerLogin();
                    while (1) {
                        cout << "\n waht would you like to do today " << manName << endl;
                        cout << " [1] SEE WORK REQUESTS FOR YOU " << endl << " [2] COMPLETE SPECIFIC WORK REQUESTS " << endl<< " [3] VIEW TEAM MEMBERS " << endl << " [4] ASSIGN TASK TO TEAM MEMBER " << endl << " [0] LOGOUT" << endl;
                        int Memberchoice = 0;
                        cin >> Memberchoice;
                        if (Memberchoice == 1) {
                            NewManager->viewWorkRequests(members);
                        }
                        if (Memberchoice == 2) {
                            NewManager->completeTask();
                        }
                        if (Memberchoice == 3) {
                            NewManager->viewTeamMembers();
                          ;
                        }
                        if (Memberchoice == 4) {
                            NewManager->assignTask();
                            ;
                        }
                        if (Memberchoice == 0) {
                            NewManager->ManagerLogOut();
                            cout << "\n\n\t\t BYE BYE\t\t\n\n";
                            break;
                        }
                    }

                }
            }
        }
        else if (choice == 3) {
            cout << "\n WELCOME TEACHER \n" << endl << " Please LOGIN by Entering your Name and id  " << endl << " Name: ";
            string manName;
            int manid;
            cin >> manName;
            cout << endl << " ID: ";
            cin >> manid;
            int chk = 0;

            TeacherInterface* NewTeacher = nullptr;
            int memberIndex = -1;

            for (int i = 0; i < teachers.size(); ++i) {
                if (teachers[i].getid() == manid && teachers[i].getname() == manName) {
                    memberIndex = i;
                    chk = 1;
                    cout << "\n WELCOME " << manName << endl;
                    break;
                }
            }

            if (!chk) {
                cout << "\n NO SUCH TEACHER EXISTS PLEASE ASK THE ADMINISTRATOR TO ADD YOU ";
            }
            else {
                // Now, NewMember points to the original object in the vector
                NewTeacher = &teachers[memberIndex];
                NewTeacher->TeacherLogin();
                while (1) {
                    cout << "\n waht would you like to do today " << manName << endl;
                    cout << " [1] SEE ALL YOUR REQUESTS " << endl << " [2] PROVIDE FEEDBACK TO SPECIFIC WORK REQUESTS " << endl << " [3] SUBMIT A NEW REQUEST/COMPLAIN" << endl<< " [0] LOGOUT" << endl;
                    int Memberchoice = 0;
                    cin >> Memberchoice;
                    if (Memberchoice == 1) {
                        NewTeacher->showRequests();
                    }
                    if (Memberchoice == 2) {
                        
                        NewTeacher->provideFeedback(teams);
                    }
                    if (Memberchoice == 3) {
                        NewTeacher->submitWorkRequest(teams);
                        ;
                    }
                    if (Memberchoice == 0) {
                        NewTeacher->TeacherLogOut();
                        cout << "\n\n\t\t BYE BYE\t\t\n\n";
                        break;
                    }
                }

            }
            }
        else if (choice == 4) {
            cout << "\n WELCOME ADMINISTRATOR \n" << endl << " Please LOGIN by Entering your Name and id  " << endl << " Name: ";
            string manName;
            int manid;
            cin >> manName;
            cout << endl << " ID: ";
            cin >> manid;
            int chk = 0;

            Administrator* adminInstance = Administrator::getInstance(0,"Admin",teams, teachers, members);
            //Admin 0
            adminInstance->AdminLogin(manid, manName);
            
                // Now, NewMember points to the original object in the vector
                while (1) {
                    cout << "\n waht would you like to do today " << manName << endl;
                    cout << " [1] viewWorkRequestsSummaryS " << endl << " [2] viewTeamWorkRequests " << endl << " [3] addTeacher" << endl
                        << " [4] addTeamMember" << endl << " [5] showTeamMembers" << endl << " [6] DeleteTeamMember" << endl << " [7] showTeams" << endl << " [8] DeleteTeam" << endl << " [9] showTeachers" << endl << " [10] DELETE TEACHER " <<endl << " [10] DELETE MANAGER " <<endl << " [11] ADD MANAGER " << endl << " [0] LOGOUT" << endl;
                    int Memberchoice = 0;
                    cin >> Memberchoice;
                    if (Memberchoice == 1) {
                        adminInstance->viewWorkRequestsSummary();
                    }
                    if (Memberchoice == 2) {

                        adminInstance->viewTeamWorkRequests();
                    }
                    if (Memberchoice == 3) {
                        adminInstance->addTeacher();
                        
                    }
                    if (Memberchoice == 4) {
                        adminInstance->addTeamMember();

                    }
                    if (Memberchoice == 5) {
                        adminInstance->showTeamMembers();

                    }
                    if (Memberchoice == 6) {
                        adminInstance->DeleteTeamMember();

                    }
                    if (Memberchoice == 7) {
                        adminInstance->showTeams();

                    }
                    if (Memberchoice == 8) {
                        adminInstance->DeleteTeam();

                    }
                    if (Memberchoice == 9) {
                        adminInstance->showTeachers();

                    }
                    if (Memberchoice ==10 ) {
                        adminInstance->DeleteTeacher();

                    }
                    if (Memberchoice == 11) {
                        adminInstance->DeleteManager();

                    }
                    if (Memberchoice == 12) {
                        adminInstance->AddManager();

                    }
                    if (Memberchoice == 0) {
                        adminInstance->AdminLogout(teams, members, teachers);
                        cout << "\n\n\t\t BYE BYE\t\t\n\n";
                        break;
                    }
                }

            }
            
        else if (choice == 0) {break;}
        else { cout << "\n INVALID INPUT TRY AGAIN \n"; }

    }
    dataManager.saveData(teams, members, teachers);

    return 0;
}
