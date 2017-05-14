package main

import (
	//"encoding/json"
	"fmt"
	//"io"
	//"io/ioutil"
	"log"
	"net/http"
	//"os"
	//"os/exec"
	"sync"
	//"time"
)

type tmnode struct {
	name string
	//neighbor []string
	//start    bool
	//accept   bool
}

func runTM(arg tmnode, filepath string, wg *sync.WaitGroup) {
	fmt.Println("NOT YET READY")
}

var turingMachineModel = tmnode{}

func init() {
	//var turingMachineModel = tmnode{}
}

func main() {
	fs := http.FileServer(http.Dir("./public"))
	http.Handle("/", fs)
	http.HandleFunc("/submit", func(w http.ResponseWriter, r *http.Request) {
		r.ParseForm()
		//for index, value := r.FormValue
		fmt.Println("Hello World")
		fmt.Println(r.Form)
		turingMachineModel.name = r.FormValue("data")
		var runningtm sync.WaitGroup
		runningtm.Add(1)
		runTM(turingMachineModel, "./public/json/data.json", &runningtm)
		runningtm.Wait()
		//runTM(turingMachineModel, )
		http.Redirect(w, r, "/", 301)
	})
	log.Println("Listening...")
	log.Fatal(http.ListenAndServe(":3000", nil))
}
