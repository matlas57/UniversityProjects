import java.util.*;

class Assignment implements Comparator<Assignment>{
	int number;
	int weight;
    int deadline;
    int completiontime;
	
	
	protected Assignment() {
	}
	
	protected Assignment(int number, int weight, int deadline, int completiontime) {
		this.number = number;
		this.weight = weight;
        this.deadline = deadline;
        this.completiontime = completiontime;
	}
	
	
	
	/**
	 * This method is used to sort to compare assignment objects for sorting. 
	 * Return -1 if a1 > a2
	 * Return 1 if a1 < a2
	 * Return 0 if a1 = a2 
	 */
	@Override
	public int compare(Assignment a1, Assignment a2) 
	{
		if (a1.weight/a1.completiontime > a2.weight/a2.completiontime)
		{
			return -1;
		}
		else if (a1.weight/a1.completiontime < a2.weight/a2.completiontime)
		{
			return 1;
		}
		else 																	//if weight per hour is the same for two events sort by deadline
		{
			if (a1.deadline < a2.deadline)
			{
				return -1;
			}
			else if (a1.deadline > a2.deadline)
			{
				return 1;
			}
			else 
			{
				return 0;
			}
		}
	}
}

public class HW_Sched {
	ArrayList<Assignment> Assignments = new ArrayList<Assignment>();
	int m;
    int lastDeadline = 0;
    double grade = 0.0;
	
	protected HW_Sched(int[] weights, int[] deadlines, int[] completiontimes, int size) throws Exception {
        if(size==0){
            throw new Exception("There is no assignment.");
        }
		for (int i=0; i<size; i++) {
			Assignment homework = new Assignment(i, weights[i], deadlines[i], completiontimes[i]);
			this.Assignments.add(homework);
			if (homework.deadline > lastDeadline) {
				lastDeadline = homework.deadline;
			}
		}
		m =size;
	}
	
	public static void main(String[] args) 
	{
		int[] weights = {40, 20, 40};
		int[] deadlines = {2, 1, 7};
		int[] times = {2, 1, 5};
		try
		{
			HW_Sched test = new HW_Sched(weights, deadlines, times, 3);
//			for (Assignment a : test.Assignments)
//			{
//				System.out.println(a.weight + " " + a.deadline + " " + a.completiontime);
//			}
			Collections.sort(test.Assignments, new Assignment());
			System.out.println();
//			for (Assignment a : test.Assignments)
//			{
//				System.out.println(a.weight + " " + a.deadline + " " + a.completiontime);
//			}
			ArrayList<Integer> result = test.SelectAssignments();
			for (Integer i : result)
			{
				System.out.print(i + " ");
			}
			System.out.println();
			System.out.println(test.grade);
		}
		catch (Exception e)
		{
			System.out.println("Silly Rabbit");
		}
	}
	
	
	/**
	 * 
	 * @return Array where output[i] corresponds to the assignment 
	 * that will be done at time i.
	 */
	public ArrayList<Integer> SelectAssignments() 
	{	
		//Sort assignments
		//Order will depend on how compare function is implemented
		Collections.sort(Assignments, new Assignment());
		// If homeworkPlan[i] has a value -1, it indicates that the 
		// i'th timeslot in the homeworkPlan is empty
		ArrayList<Integer> homeworkPlan = new ArrayList<>();
		for (int i=0; i < lastDeadline; ++i) 
		{
			homeworkPlan.add(-1);
		}
		int nextFree = 0; 										// an index variable to indicate where the next free slot in the schedule is
		for (Assignment asgn : this.Assignments) 				// iterate through the assignments
		{
			if (nextFree < asgn.deadline)							// if the deadline for asgn hasnt passed
			{
				int time = asgn.completiontime;
				for (int i = 0; i < time; i++)							// go through the time to complete the assignment and add it to schedule
				{		
					if (nextFree + i >= this.lastDeadline)					// if adding the next hour would go over the last deadline
					{
						int dif = time - i;										// find how late the assignment is 
						if (dif > 0)
						{
							this.grade += (asgn.weight - (asgn.weight * (dif * .1))); // deduct points for how late
						}
						return homeworkPlan;										  // return since end was reached	
					}
				homeworkPlan.set(nextFree + i, asgn.number);				// add the hour to the schedule
				}
				nextFree += asgn.completiontime;						// increment the nextFree so that we start at the correct index for the next assignment
				// calculating the points received for the assignment
				int dif = nextFree - asgn.deadline;						
				if (dif > 0)											// if the assignment was late
				{
					this.grade += (asgn.weight - (asgn.weight * (dif * .1)));	// deduct points for how late
				}
			else 														// if the assignment is on time
				{
					this.grade += asgn.weight;									// grant full points
				}
			}
		}
		return homeworkPlan;
	}
}
	



