plot_tt_hr <- function(file='',title=''){

   print(file)
   print(title)
   con <- file(file, "r")
   lines <- readLines(con = file, n = -1)

   ni=length(lines)
   print(paste(ni,"lines found in file ",file))

   xx=seq(1,ni)
   tt=seq(1,ni)
   hr=seq(1,ni)

   #"201506212030, Coin SE : temperature 20 C, humidite 61%"
   #                        123456789012345678901234567890
   #                        0        1         2         3
   # Find index of since name (Coin SE) can of any length :
   found = 0
   ind=0
   while ( found != 1 ) {     
      ind = ind +1
      if( substring(lines[1],ind,ind) == ':' ){
         found = 1
      }
   }
   #print(ind)

   for ( i in seq(1,ni) ){
      tt[i]=strtoi(substring(lines[i],ind+14,ind+15))
      hr[i]=strtoi(substring(lines[i],ind+29,ind+30))
   }

   plot(xx,tt)
   #plot(xx,hr)

}