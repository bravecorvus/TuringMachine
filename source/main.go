package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	//"os"
	"os/exec"
	"strconv"
	"strings"
)

type edge struct {
	vertex1   int64
	vertex2   int64
	operation string
}

func runTM(arg string, filepath string) {
	var statesEnd int = 0
	strArray := strings.Split(arg, "")
	for _, val := range strArray {
		if val == "}" {
			statesEnd++
			break
		} else {
			statesEnd++
		}
	}

	//EXTRACT TRANSITION STATES
	states := strArray[5:statesEnd]
	var alphabet []string
	var inalphabetlist bool
	for _, i := range states {
		//fmt.Println(i)
		inalphabetlist = false
		if (i != "{") && (i != ",") && (i != "}") && (i != "L") && (i != "R") && (i != "r") && (i != "l") && (i != " ") && (i != "S") {
			for _, j := range alphabet {
				if j == i {
					inalphabetlist = true
					break
				}
			}
			if !inalphabetlist {
				alphabet = append(alphabet, i)
			}
		}
	}

	//ALPHABET IS A ARRAY WHICH STORES ALL THE UNIQUE ALPHABET OF THE TM
	strArray = strArray[statesEnd+3 : len(strArray)]
	statesEnd = 0
	var newiter int = 0

	//COUNT NUMBER OF STATES
	for _, i := range strArray {
		if i == "}" {
			break
		} else {
			if i == "q" {
				statesEnd++
			}
		}
		newiter++
	}

	strArray = strArray[newiter+3 : len(strArray)]
	newiter = 0
	var edgestorage []string
	var index int = 0

	for {
		if strArray[index] == "}" {
			strArray = strArray[2:]
			break
		} else {
			if strArray[index] == ")" {
				edgestorage = append(edgestorage, strings.Join(strArray[2:index], ""))
				strArray = strArray[index+2:]
				index = 0
			} else {
				index++
			}
		}
	}

	index = 0

	for _, i := range strArray {
		if i == "," {
			break
		} else {
			index++
		}
	}
	//startstate, err := strconv.ParseInt(strings.Join(strArray[1:index], ""), 10, 64)
	//if err != nil {
	//fmt.Println("ERROR")
	//}
	strArray = strArray[index+3:]
	index = 0

	for _, i := range strArray {
		if i == "}" {
			break
		} else {
			index++
		}
	}
	acceptstate, err := strconv.ParseInt(strings.Join(strArray[1:index], ""), 10, 64)
	if err != nil {
		fmt.Println("ERROR")
	}
	//startstate += 1
	//acceptstate += 1
	//fmt.Println(startstate)
	//fmt.Println(acceptstate)
	var parser []string
	temp := edge{}
	var edgestructstore []edge
	for _, i := range edgestorage {
		parser = strings.Split(i, ", ")
		v1str := strings.Join(strings.Split(parser[0], "")[1:], "")
		v1, err := strconv.ParseInt(v1str, 10, 8)
		if err != nil {
			fmt.Println("ERROR")
		}
		temp.vertex1 = v1
		v2str := strings.Join(strings.Split(parser[1], "")[1:], "")
		v2, err := strconv.ParseInt(v2str, 10, 8)
		if err != nil {
			fmt.Println("ERROR")
		}
		temp.vertex2 = v2
		temp.operation = parser[2]
		edgestructstore = append(edgestructstore, temp)
	}

	tmstructure := make([][]string, statesEnd)
	for i := 0; i < len(tmstructure); i++ {
		tmstructure[i] = make([]string, len(alphabet))
	}
	for i := 0; i < statesEnd; i++ {
		for j := 0; j < len(alphabet); j++ {
			tmstructure[i][j] = "_"
		}
	}
	preconvertedstring := make([]string, 3)
	var splitoperations []string
	var convertedstring string
	var matchindex int

	for _, i := range edgestructstore {
		splitoperations = strings.Split(i.operation, "")

		for indexx, j := range alphabet {
			if splitoperations[0] == j {
				matchindex = indexx
				break
			}
		}
		preconvertedstring[0] = splitoperations[1]
		if splitoperations[2] == "L" || splitoperations[2] == "l" {
			preconvertedstring[1] = "-1"
		} else if splitoperations[2] == "R" || splitoperations[2] == "r" {
			preconvertedstring[1] = "1"
		} else if splitoperations[2] == "S" || splitoperations[2] == "s" {
			preconvertedstring[1] = "0"
		}
		preconvertedstring[2] = strconv.FormatInt(int64(i.vertex2), 10)
		convertedstring = strings.Join(preconvertedstring, " ")
		//fmt.Println("TRANSITION STATE", i.operation)
		//fmt.Println("ALPHABET", strings.Join(alphabet, ""))
		//fmt.Println("VERTEX1", i.vertex1)
		//fmt.Println("MATCHINGINDEX", matchindex)
		tmstructure[i.vertex1][matchindex] = convertedstring
	}
	//for yolo2 := 0; yolo2 < len(alphabet); yolo2++ {
	//fmt.Println(tmstructure[yolo1][yolo2])
	//}
	//fmt.Println("\n")
	//}
	//fmt.Println("\n\n")
	var rawstring = []string{strconv.Itoa(int(statesEnd)), "\n", strings.Join(alphabet, ""), "\nq\n\n"}
	//fmt.Println(strings.Join(rawstring, ""))
	for i := 0; i < statesEnd; i++ {
		if i == int(acceptstate) {
			rawstring = append(rawstring, "A\n")
		} else {
			rawstring = append(rawstring, "R\n")
		}
		for j := 0; j < len(alphabet); j++ {
			if j == len(alphabet)-1 {
				rawstring = append(rawstring, tmstructure[i][j])
			} else {
				rawstring = append(rawstring, tmstructure[i][j])
				rawstring = append(rawstring, "\n")
			}
		}
		if i != statesEnd-1 {
			rawstring = append(rawstring, "\n")
			rawstring = append(rawstring, "\n")
		} else if i == statesEnd-1 {
			rawstring = append(rawstring, "\n")
		}
	}
	error1 := ioutil.WriteFile(filepath, []byte(strings.Join(rawstring, "")), 0644)
	if error1 != nil {
		fmt.Println("ERROR")
	}
}

var rawdata []string

func main() {

	fs := http.FileServer(http.Dir("./public"))
	http.Handle("/", fs)

	http.HandleFunc("/submit", func(w http.ResponseWriter, r *http.Request) {
		rawdata = rawdata[:]
		r.ParseForm()
		for value, _ := range r.Form {
			rawdata = append(rawdata, value)
		}
		runTM(rawdata[0], "./TM.txt")
		http.Redirect(w, r, "/", 301)
	})

	http.HandleFunc("/testing", func(w http.ResponseWriter, r *http.Request) {
		//rawdata = rawdata[0:0]
		ajax_post_data := r.FormValue("ajax_post_data")
		fmt.Println("Receive ajax post data string ", ajax_post_data)
		var status string
		//for key, _ := range r.Form {
		//rawdata = append(rawdata, key)
		//}
		//fmt.Println(rawdata[0])
		//var (
		//cmdOut []byte
		//err    error
		//)
		cmdName := "./TM"
		cmdArgs := []string{"TM.txt", ajax_post_data}
		cmd := exec.Command(cmdName, cmdArgs...)
		cmdReader, err := cmd.StdoutPipe()
		if err != nil {
			//fmt.Fprintln(os.Stderr, "rejected")
			status = "rejected"
			//os.Exit(1)
		}
		scanner := bufio.NewScanner(cmdReader)
		go func() {
			for scanner.Scan() {
				//fmt.Printf("%s\n", scanner.Text())
				status = scanner.Text()
			}
		}()
		err = cmd.Start()
		if err != nil {
			status = "rejected"
			//fmt.Fprintln(os.Stderr, "rejected")
			//os.Exit(1)
		}
		err = cmd.Wait()
		if err != nil {
			status = "rejected"
			//fmt.Fprintln(os.Stderr, "rejected")
			//os.Exit(1)
		}
		//error2 := ioutil.WriteFile("./currentState.conf", []byte(status), 0644)
		//if error2 != nil {
		//fmt.Println("ERROR")
		//}
		w.Write([]byte(status))
	})
	log.Println("Listening...")
	log.Fatal(http.ListenAndServe(":3000", nil))
}
