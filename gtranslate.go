package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"os"
	"strings"
)

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s \"text\"\n", os.Args[0])
		os.Exit(1)
	}

	Url := "http://translate.googleapis.com/translate_a/single?client=gtx&sl=auto&tl=ru&dt=t&"
	Ua := "Mozilla/5.0"

	client := &http.Client{}
	s := make(url.Values)
	s.Add("q", os.Args[1])
	Url = Url + s.Encode()
	req, err := http.NewRequest("GET", Url, nil)
	if err != nil {
		log.Fatal(err)
	}
	req.Header.Set("User-Agent", Ua)
	resp, err := client.Do(req)
	if err != nil {
		log.Fatal(err)
	}
	data, err := ioutil.ReadAll(resp.Body)
	resp.Body.Close()
	if err != nil {
		log.Fatal(err)
	}

	length := len(data)
	data2 := make([]byte, length)
	for j, i := 0, 0; i < length; i++ {
		if data[i] != '\n' {
			data2[j] = data[i]
			j++
		}
	}
	str := string(data2[4:])
	length = len(str)
	for i := 0; i < length; i++ {
		if n := strings.Index(str[i:], "\",\""); n != -1 {
			if i == 0 {
				fmt.Fprintf(os.Stdout, "%s", str[i:i+n])
			}
			if m := strings.Index(str[i:i+n], "],["); m != -1 {
				for _, t := range str[i+m+4 : i+n] {
					if t >= 1000 {
						fmt.Fprintf(os.Stdout, "%s",
							str[i+m+4:i+n])
						break
					}
				}
			}
			i += n + 3
		} else {
			break
		}
	}
	fmt.Println()
}
