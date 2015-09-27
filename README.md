# WebQuest-Interpreter
A interpreter written in C++ for debugging and texting HTTP Request, RESTful Services.

Current Achievement:

	Runtime rt;
	rt.Run("print('asdf')");
	rt.Run("a='yea' b=true print(b)");
	rt.Run("if true a='yes' else a='no' end print(a)");
	rt.Run("a=0 while a<10  a=a+1 print(a) end");
	rt.Run("aa=[234,234,234] append(aa,'aa') append(aa,'bb') append(aa,123)  print(aa)");
	rt.Run("aa=['wer'] aa[0]=1 print(aa) ");
	rt.Run("aa=0 aa=aa==2+2-4 print(aa)");
	rt.Run("aa=1==2+5*3 print(aa)");
	rt.Run("result=get(\"http://higavin.com\") print(result)");
	rt.Run("if (0==4-4+2+1-3 && true==1) print('good!') else print('bad') end ");
    rt.Run("lsvar=[123,456,789] for a in lsvar print(a+' ') break end");
	rt.Run("a=[123,456,789] print(a[0:2]) print(a[0:-1])");

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
        a.
        
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
        end
        
        Example:
            condi=5
            if cond>5
                print("greater than 5")
            elseif codi<5
                print("less than 5")
            else
                print("equal to 5")
            end
            
    b.while loop
        Format:
            while <bool expression>
                <statement>
                    .
                    .
                    .
            end
        
        Example:
            a=1
            while a<10
                print(a)
                a+=1
            end
    c.for loop
        Format:
            for <variable> in <expression>
                <statement>
                    .
                    .
                    .
            end
        Example:
            lsval=[123,456,789]
            for i in lsval
                print(i)
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
        c. list modification
            Format:
                <list variable>[<index expression>]= <expresion>
            Example:
                lsvar=[12,31,55]
                lsvar[0]=16
    5. Dictionary Operation

Important Functions
    1. get(url[,data[,headers]])
    2. post(url[,data[,headers]])