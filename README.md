# GraduationProjectManagerSystem
数据库原理课程设计——毕业设计管理系统



**功能简介：**

１.根据题目的描述，既然这个系统有教师和学生的管理，那这个系统不仅是毕业设计管理系统，而是有教师和学生的信息查询维护的教务管理系统的一部分。 本系统是一个专门用于毕业设计管理的系统，每个准备毕业设计的学生和每个指导老师都拥有一个账号。该系统的账号可能是从教务管理系统导入进来的。

２.鉴于审批需要，该系统共有四种角色，分别为教师、学生、系主任、管理员。不同的用户登录到这个系统中要有不同的界面，不同的功能。 

3.学生界面内有“选题”功能，在选择题目并经导师和系主任批准后，将开题并可以在“上传进度”中实时查看自己的进度，随时补充最新进展。

4.系统管理员具有增删用户和决定用户权限的功能，但系统管理员不能涉及选题与审核环节，要修改选题与审核等环节的内容，需要管理员为自己创建具有系主任权限的教师账号。

5.没有系主任权限的教师只能指导学生和开题，不能进入系主任审批界面。而具有系主任权限的教师可以进入系主任审批界面，也可以指导学生和开题。（值得一提的是，系主任可以审批自己指导的学生进行的毕业设计。）

**逻辑结构设计：**（加粗表示主键） 

用户表（**统一ID**，密码，用户类别）

学生表（**统一ID**，姓名，性别，专业，班级，电话，邮箱，备注）

教师表（**统一ID**，姓名，性别，职称，方向，电话，邮箱，是否系主任）

题目表（**题目编号**，题目名称，题目专业，命题导师，内容简介）

选题表（**题目编号**，学生ID，教师ID，毕设进程）

**开发所用技术与环境：** 

架构：native

语言：C++

数据库：sqlite 3.31.1

使用的库：EasyX_20200520(beta)

开发系统：Windows 10.0 Build 18362

开发工具：Visual Studio 2017 Community

支持的系统：Windows 7/10 AnyCPU（指能跑Windows的）

**需要说明的内容：**

需要说明，本次管理系统对学生的毕设进程进行了简化

分为0，1，2，3，4，5共计6个阶段

其中，系主任开题审批只针对0阶段

教师的审核（中期检查和导师意见）仅针对1，2阶段

系主任的审批（系主任审批和毕业答辩）仅针对3，4阶段

第5阶段为顺利毕业。

**安全性考虑：**

在登陆过程中对于是否为管理员采用的是预先与储存好的匹配而不是直接进行查询语句，避免了在用户登录过程中被SQL注入的风险，如admin'#这样的常见SQL注入点被避免。

而诸如''or 1=1#或username' AND 1=1—hack这样的注入方式，或者更为恶劣的username;DROP TABLE user—hack的攻击，采用对请求的字符串预处理的形式，过滤其中可提供多语句执行的;和=两个常见字符，在尽可能减少对用户自由性损失的同时，防范SQL注入的攻击。

非常遗憾因为时间关系，密码没能采用加盐后HASH，然后将HASH后数据进行比对的较为安全的方式进行处理，而是直接将明文送入查询。


=====================ZH_CN===================

**Function introduction:**

1. According to the description of the topic, since this system has the management of teachers and students, then this system is not only a graduation design management system, but also a part of the educational management system with information query and maintenance of teachers and students. This system is a system dedicated to graduation design management. Every student preparing for graduation design and each instructor have an account. The account of the system may be imported from the educational administration management system.

2. In view of the need for approval, the system has four roles, namely teacher, student, department head, and administrator. Different users have different interfaces and different functions when logging into this system.

3. There is a "select topic" function in the student interface. After selecting the topic and approval by the tutor and department head, the topic will be opened and you can view your progress in real time in the "upload progress", and supplement the latest progress at any time.

4. The system administrator has the functions of adding and deleting users and determining user rights, but the system administrator cannot involve the topic selection and review. To modify the content of the topic selection and review, the administrator needs to create a teacher with the authority of the department head for himself account number.

5. Teachers without department head authority can only guide students and open questions, and cannot enter the department head approval interface. Teachers who have the authority of the department head can enter the department head approval interface, and can also guide students and open questions. (It is worth mentioning that the dean of the department can approve the graduation design of students under his guidance.)

**Logical structure design:** (Bold indicates the primary key)

User table (**uniform ID**, password, user category)

Student Form (**Uniform ID**, name, gender, major, class, phone, email, remarks)

Teacher Form (**Uniform ID**, name, gender, job title, direction, telephone, email, whether or not the department head)

Question List (**Question Number**, Title Name, Subject Specialty, Proposition Tutor, Brief Introduction)

Selection table (**topic number**, student ID, teacher ID, completion process)

**Technology and environment used for development:**

Architecture: native

Language: C++

Database: sqlite 3.31.1

Library used: EasyX_20200520(beta)

Development system: Windows 10.0 Build 18362

Development tools: Visual Studio 2017 Community

Supported system: Windows 7/10 AnyCPU (means able to run Windows)

**What needs to be explained:**

It needs to be explained that this management system simplifies the process of student completion

Divided into 0, 1, 2, 3, 4, 5 total 6 stages

Among them, the dean of the department opens the examination and approval only for stage 0

The teacher's review (interim inspection and tutor's opinion) is only for stage 1, 2

Department head approval (Department Head Approval and Graduation Reply) is only for stages 3 and 4

The fifth stage is a successful graduation.

**Security considerations:**

During the login process, whether to use the stored match in advance for the administrator instead of directly querying the query avoids the risk of SQL injection during the user login process. Common SQL injection points such as admin'# are avoided .

Injecting methods such as'or 1=1# or username' AND 1=1—hack, or the worse username; DROP TABLE user-hack attacks, use the form of preprocessing the requested string, filtering Which can provide multi-statement execution; and = two common characters, while reducing the loss of user freedom as much as possible, while preventing SQL injection attacks.

Unfortunately, due to time constraints, the password failed to use HASH after salting, and then the post-HASH data was processed in a more secure way, but the plain text was directly sent to the query.


=====================EN_UK===================