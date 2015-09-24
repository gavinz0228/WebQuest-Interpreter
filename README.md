# WebQuest-Interpreter
A interpreter written in C++ for debugging and texting HTTP Request, RESTful Services.

Language Usage:
    
    
    1. Assignment
        Tab, white space and new line are seen as the same.
        Format:
            <variable>=<expression>
        Example:
            str="string"
            intvar=123
            fltvar=123.321
            funvar=funcname(param1,param2)
            expvar="string1"+"string2"
            boolvar=true
            boolvar=3==3
            
    2. Function Call
        Format:
            <function>(<variable>,<variable>,<variable>...)
        Example:
            func("abc",1+2,var1)
            print("abc")
            
    3. Flow Control
        if <boolean expression> 
            <statements>
            <statements>
                .
                .
                .
        [else if <boolean expression> 
            <statements>
            <statements>
                .
                .
                .   
        ]
        [
        else
            <statements>
            <statements>
                .
                .
                .  
        ]
        
    4. List Operation 
        a. list assignment
            Format:
                <variable>=[<expression>,<expression>,<expression>...]
                <variable>=[]
            Examples:
                lsvar=[123,"123",12.3]
                lsvar=[]
                
        b. appending a element to a list
            Format:
                append(<list variable>,<new element>)
            Example:
                var ls=[]
                append(ls,"asdf")
    5. Dictionary Operation

Important Functions
    1. get(url[,data[,headers]])
    2. post(url[,data[,headers]])