# WebQuest-Interpreter
A interpreter written in C++ for debugging and texting HTTP Request, RESTful Services.

Current Achievement:

	Runtime rt;
		rt.Run("print('asdf')");
		rt.Run("a='yea' b=true print(b)");
		rt.Run("if true a='yes' else a='no' end print(a)");
		rt.Run("a=0 while a<10  a=a+1 print(a) end");
		rt.Run("aa=[234,234,234] append(aa,'aa') append(aa,'bb') append(aa,123)  print(aa)");
		rt.Run("aa=['stringtest'] aa[0]=1 print(aa) ");
		rt.Run("aa=0 aa=aa==2+2-4 print(aa)");
		rt.Run("aa=1==2+5*3 print(aa)");
		rt.Run("result=get(\"http:higavin.com\") print(result)");
		rt.Run("变量=1 变量+ =1 print(变量)");
		rt.Run("if (0==4-4+2+1-3 && true==1) print('good!') else print('bad') end ");
		rt.Run("a=1 while a<=10 b=2 a+=1 a+=1 print(a)  end print(a)");
		rt.Run("a=1 a+=1 print(a)");
		rt.Run("lsvar=[123,456,789] for a in lsvar print(a) end");
		rt.Run("lsvar=[123,456,789] for a in lsvar print(a+' ') break end");
		rt.Run("a=[123,456,7890.02] print(a[0:3]) print(a[0:-1])");
		rt.Run("res=get_raw('http:google.com') print(len(res))");
		rt.Run("res=get('http:google.com') print(res) #print(res)");
		rt.Run("aa={} aa['key']='value' dump_json(aa)");
		rt.Run("for i in range(0,10) print(i) end");
		rt.Run("for i in range(10) print(i) end");
		rt.Run("res=get('https://maps.googleapis.com/maps/api/geocode/json?address=2900+bedford+avenue+brooklyn') print(res)");
		rt.Run("res=get_raw('https://maps.googleapis.com/maps/api/geocode/json?address=2900+bedford+avenue+brooklyn') \
			    header=parse_headers(res) print(header) \
				print('---------------------------------------\n') \
			");

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
        d. list slicing
            Format:
                <list variable>[<start index>:<end index>]
                    from <start index> to <end index>
                <list variable>[:<end index>]
                    from the beginning to <end index>
                <list variable>[<start index>:]
                    from the <start index> to the end
                <list variable>[:]
                    a copy of the original list
    5. Dictionary Operation
Important Features
    1.List Slicing(string type and list type)
    
    
Core Functions

    Basic Functions: 
        len(object):
            Gets the length of a list object or string object. If the parameter is a string, it will return the number of characters of the string. If the parameter is a list, it will return the number of elements of the list.
        print(object)
            Print out object to the standard output. Non-string object will be converted to string before being printed.
        append(object,element)
            Appends an element to a list object.
    Web Related Functions:
        get(url[,data[,headers]])
            Sends a get request, and only returns the body of the response returned by server. Parameters data is a dictionary variable which will be url-encoded. Parameters header is also a dictionary variable.
        get_raw(url[,data[,headers]])
            Sends a get request, and return the complete string returned by server. Function signature is the same as function get . 
        post(url[,data[,headers]])
    