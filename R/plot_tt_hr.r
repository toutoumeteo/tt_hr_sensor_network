plot_tt_hr <- function(file='',title=''){

	   print(paste(title,'.png',sep=''))

   png(filename = paste(title,'.png',sep=''),width = 480, height = 300, units = "px", pointsize = 12, bg = "white")
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

   plot(xx,tt,ylim=c(10,30), type = "l", col='blue', ylab = "Temperature", main=title)
   par(new = TRUE)
   plot(xx, hr, ylim=c(20,100), type = "l", col='green', axes = FALSE, bty = "n", xlab = "", ylab = "")
   axis(side=4)
   mtext("HR", side=4, line=3)

}