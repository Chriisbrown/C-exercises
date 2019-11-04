# demonstrate template string functions
import string

def main():
    # Usual string formatting with format()
    str1 = "You're watching {0} by {1}".format("Advanced Python", "Joe Marini")
    print(str1)
    
    # TODO: create a template with placeholders
    templ = string.Template("watching ${title} by ${author}")
    str2 = templ.substitute(title="Python",author="me")
    

    data = {
        "author" : "me",
        "title" : "python"

    }
    str3 = templ.substitute(data)
    print(str3)


    # TODO: use the substitute method with keyword arguments
    
    # TODO: use the substitute method with a dictionary

    
if __name__ == "__main__":
    main()
    